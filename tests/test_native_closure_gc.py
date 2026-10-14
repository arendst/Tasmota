#!/usr/bin/env python3
"""Exercise real Berry native-closure construction and GC under allocation pressure.

Builds the actual runtime in a temporary directory. Only be_func.c is compiled
with object/allocation hooks: fresh closure slots are poisoned like reused heap.
At a chosen upvalue boundary, raw allocations fill remaining small-pool slots;
the next OS allocation for a new pool fails once or twice. The actual allocator
performs GC and retries or throws. All marking, protected-call unwinding,
closure destruction and allocation/retry behavior are real.

Run: python3 tests/test_native_closure_gc.py --check-baseline e2821e8
"""
from __future__ import annotations
import argparse
from concurrent.futures import ThreadPoolExecutor
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
BERRY = ROOT / "lib/libesp32/berry"
CORE = BERRY / "src"
HARNESS = r'''
#include "berry.h"
#include "be_func.h"
#include "be_gc.h"
#include "be_mem.h"
#include "be_vm.h"
#include "be_exec.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int armed, allocation_index, collect_at, permanent_failure, collections;
static int failures_remaining, injected_failures;
static size_t failed_allocation_size;
static void *pressure_blocks[128];
static int pressure_count;
static int upvalue_count;
static bntvclos *constructing;

bgcobject *test_newgcobj(bvm *vm, int type, size_t size) {
    bgcobject *obj = be_newgcobj(vm, type, size);
    if (armed && type == BE_NTVCLOS) {
        constructing = (bntvclos *)obj;
        size_t count = (size - sizeof(bntvclos)) / sizeof(bupval *);
        /* Deterministic reused-memory contents, before constructor initializes it. */
        for (size_t i = 0; i < count; ++i) {
            be_ntvclos_upval(constructing, i) = (bupval *)(uintptr_t)1;
        }
    }
    return obj;
}

/* Only be_mem.c substitutes this function for its OS malloc calls. */
void *test_malloc(size_t size) {
    if (failures_remaining) {
        --failures_remaining;
        ++injected_failures;
        failed_allocation_size = size;
        return NULL;
    }
    return malloc(size);
}

void *test_realloc(bvm *vm, void *ptr, size_t old_size, size_t new_size) {
    if (armed && constructing && !ptr && !old_size && new_size == sizeof(bupval)) {
        ++allocation_index;
        if (allocation_index == collect_at) {
            ++collections;
            /* Fill this real allocator size class until it needs a new pool.
             * Fail its OS allocation: be_realloc itself collects and retries. */
            failures_remaining = permanent_failure ? 2 : 1;
            while (1) {
                void *block = be_realloc(vm, NULL, 0, new_size);
                if (injected_failures) {
                    return block; /* recovered after actual allocation failure */
                }
                assert(pressure_count < 128);
                pressure_blocks[pressure_count++] = block;
            }
        }
    }
    return be_realloc(vm, ptr, old_size, new_size);
}

static int sum_upvalues(bvm *vm) {
    int sum = 0;
    for (int i = 0; i < upvalue_count; ++i) {
        be_getupval(vm, 0, i);
        sum += (int)be_toint(vm, -1);
        be_pop(vm, 1);
    }
    be_pushint(vm, sum);
    be_return(vm);
}

static int create_closure(bvm *vm) {
    be_pushntvclosure(vm, sum_upvalues, upvalue_count);
    be_return(vm);
}

int main(int argc, char **argv) {
    assert(argc == 4);
    upvalue_count = atoi(argv[1]);
    collect_at = atoi(argv[2]);
    permanent_failure = strcmp(argv[3], "oom") == 0;
    bvm *vm = be_vm_new();
    assert(vm != NULL);
    be_pushntvfunction(vm, create_closure);
    armed = 1;
    int result = be_pcall(vm, 0);
    armed = 0;
    constructing = NULL;
    assert(collections == (collect_at > 0 ? 1 : 0));
    assert(injected_failures == (collect_at ? (permanent_failure ? 2 : 1) : 0));
    if (collect_at) {
        /* Confirms failure was a new pool allocation, not a direct upvalue. */
        assert(failed_allocation_size > sizeof(bupval));
    }
    for (int i = 0; i < pressure_count; ++i) {
        be_free(vm, pressure_blocks[i], sizeof(bupval));
    }
    if (permanent_failure) {
        assert(result == BE_MALLOC_FAIL);
        /* Destroy the partially built closure after protected-call unwinding. */
        be_pop(vm, be_top(vm));
        be_gc_collect(vm);
        be_gc_collect(vm);
    } else {
        assert(result == BE_OK);
        assert(be_isntvclos(vm, -1));
        int expected = 0;
        for (int i = 0; i < upvalue_count; ++i) {
            be_getupval(vm, -1, i);
            assert(be_isnil(vm, -1));
            be_pop(vm, 1);
            be_pushint(vm, 10 + i);
            assert(be_setupval(vm, -2, i));
            be_pop(vm, 1);
            expected += 10 + i;
        }
        be_gc_collect(vm);
        assert(be_pcall(vm, 0) == BE_OK);
        assert(be_toint(vm, -1) == expected);
        be_pop(vm, be_top(vm));
        be_gc_collect(vm);
    }
    be_vm_delete(vm);
    printf("PASS upvalues=%d GC-at=%d mode=%s\n", upvalue_count, collect_at, argv[3]);
    return 0;
}
'''


def run(command: list[str], *, check: bool = True) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if check and result.returncode:
        raise RuntimeError(f"Command failed ({result.returncode}): {' '.join(command)}\n{result.stdout}")
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check-baseline", metavar="GIT_REF")
    args = parser.parse_args()
    compiler = shutil.which(os.environ.get("CC", "clang"))
    if not compiler:
        raise SystemExit("C compiler not found")
    includes = [CORE, BERRY / "default", BERRY / "generate", BERRY.parent / "re1.5",
                BERRY.parent / "berry_mapping/src", BERRY.parent / "berry_int64/src"]
    flags = ["-std=c99", "-O1", "-g", "-DUSE_BERRY_INT64", "-fno-omit-frame-pointer",
             "-fsanitize=address,undefined", "-fno-sanitize-recover=all"]
    flags += [f"-I{p}" for p in includes]
    sources = sorted(p for directory in includes if directory.name != "generate"
                     for p in directory.glob("*.c")
                     if p.name not in {"berry.c", "be_func.c", "be_gc.c", "be_mem.c"})
    # `src` occurs only once in includes; each library contributes its own sources.
    with tempfile.TemporaryDirectory(prefix="tasmota-native-closure-gc-") as temp:
        work = Path(temp)
        harness = work / "harness.c"
        harness.write_text(HARNESS)
        allocator_hook = work / "allocator_hook.h"
        allocator_hook.write_text('#include "berry.h"\n#include <stdlib.h>\n'
                                  '#undef BE_EXPLICIT_MALLOC\n'
                                  '#define BE_EXPLICIT_MALLOC test_malloc\n'
                                  'void *test_malloc(size_t size);\n')
        sources.append(harness)
        def compile_common(item: tuple[int, Path]) -> Path:
            index, source = item
            obj = work / f"common-{index}.o"
            run([compiler, *flags, "-c", str(source), "-o", str(obj)])
            return obj
        with ThreadPoolExecutor(max_workers=min(8, os.cpu_count() or 1)) as pool:
            objects = list(pool.map(compile_common, enumerate(sources)))

        def build(label: str, baseline: str | None = None) -> Path:
            specialized = []
            for name in ("be_func.c", "be_gc.c", "be_mem.c"):
                path = CORE / name
                if baseline:
                    text = run(["git", "-C", str(ROOT), "show", f"{baseline}:{path.relative_to(ROOT)}"]).stdout
                    path = work / f"{label}-{name}"
                    path.write_text(text)
                obj = work / f"{label}-{name}.o"
                hooks = []
                if name == "be_func.c":
                    hooks = ["-Dbe_newgcobj=test_newgcobj", "-Dbe_realloc=test_realloc"]
                elif name == "be_mem.c":
                    hooks = ["-include", str(allocator_hook)]
                run([compiler, *flags, *hooks, "-c", str(path), "-o", str(obj)])
                specialized.append(obj)
            binary = work / label
            run([compiler, *flags, *map(str, objects + specialized), "-lm", "-ldl", "-o", str(binary)])
            return binary

        if args.check_baseline:
            baseline = build("baseline", args.check_baseline)
            failure = run([str(baseline), "3", "1", "gc"], check=False)
            if failure.returncode == 0 or "baseline-be_gc.c:" not in failure.stdout or "0x000000000001" not in failure.stdout:
                raise RuntimeError(f"Expected baseline GC failure in mark_ntvclos, got {failure.returncode}:\n{failure.stdout}")
            print("Baseline: GC during native-closure construction dereferences an uninitialized upvalue.")

        fixed = build("fixed")
        cases = [(0, 0, "gc"), (3, 0, "gc")]
        cases += [(count, index, mode) for count in (1, 2, 3, 8)
                  for index in range(1, count + 1) for mode in ("gc", "oom")]
        for count, index, mode in cases:
            run([str(fixed), str(count), str(index), mode])
        print(f"PASS: {len(cases)} real-runtime cases cover zero upvalues, real pool-allocation GC/retry at every partial-construction boundary, callable retained values and permanent allocation-failure cleanup (ASan/UBSan).")


if __name__ == "__main__":
    main()
