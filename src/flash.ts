import { connect, ESPLoader, Logger } from "esp-web-flasher";
import { Build, FlashError, FlashState, Manifest, State } from "./const";
import { fireEvent, getChipFamilyName, sleep } from "./util";

export const flash = async (
  eventTarget: EventTarget,
  logger: Logger,
  manifestPath: string,
  eraseFirst: boolean
) => {
  let manifest: Manifest;
  let build: Build | undefined;
  let chipFamily: "ESP32" | "ESP8266" | "ESP32-S2" | "Unknown Chip";

  const fireStateEvent = (stateUpdate: FlashState) => {
    fireEvent(eventTarget, "state-changed", {
      ...stateUpdate,
      manifest,
      build,
      chipFamily,
    });
  };

  const manifestURL = new URL(manifestPath, location.toString()).toString();
  const manifestProm = fetch(manifestURL).then(
    (resp): Promise<Manifest> => resp.json()
  );

  let esploader: ESPLoader | undefined;

  try {
    esploader = await connect(logger);
  } catch (err) {
    // User pressed cancel on web serial
    return;
  }

  // For debugging
  (window as any).esploader = esploader;

  fireStateEvent({
    state: State.INITIALIZING,
    message: "Initializing...",
    details: { done: false },
  });

  try {
    await esploader.initialize();
  } catch (err) {
    logger.error(err);
    if (esploader.connected) {
      fireStateEvent({
        state: State.ERROR,
        message:
          "Failed to initialize. Try resetting your device or holding the BOOT button while selecting your serial port.",
        details: { error: FlashError.FAILED_INITIALIZING, details: err },
      });
      await esploader.disconnect();
    }
    return;
  }

  chipFamily = getChipFamilyName(esploader);

  fireStateEvent({
    state: State.INITIALIZING,
    message: `Initialized. Found ${chipFamily}`,
    details: { done: true },
  });
  fireStateEvent({
    state: State.MANIFEST,
    message: "Fetching manifest...",
    details: { done: false },
  });

  try {
    manifest = await manifestProm;
  } catch (err) {
    fireStateEvent({
      state: State.ERROR,
      message: `Unable to fetch manifest: ${err.message}`,
      details: { error: FlashError.FAILED_MANIFEST_FETCH, details: err },
    });
    await esploader.disconnect();
    return;
  }

  build = manifest.builds.find((b) => b.chipFamily === chipFamily);

  fireStateEvent({
    state: State.MANIFEST,
    message: `Found manifest for ${manifest.name}`,
    details: { done: true },
  });

  if (!build) {
    fireStateEvent({
      state: State.ERROR,
      message: `Your ${chipFamily} board is not supported.`,
      details: { error: FlashError.NOT_SUPPORTED, details: chipFamily },
    });
    await esploader.disconnect();
    return;
  }

  fireStateEvent({
    state: State.PREPARING,
    message: "Preparing installation...",
    details: { done: false },
  });

  const filePromises = build.parts.map(async (part) => {
    const url = new URL(part.path, manifestURL).toString();
    const resp = await fetch(url);
    if (!resp.ok) {
      throw new Error(
        `Downlading firmware ${part.path} failed: ${resp.status}`
      );
    }
    return resp.arrayBuffer();
  });

  // Run the stub while we wait for files to download
  const espStub = await esploader.runStub();

  const files: ArrayBuffer[] = [];
  let totalSize = 0;

  for (const prom of filePromises) {
    try {
      const data = await prom;
      files.push(data);
      totalSize += data.byteLength;
    } catch (err) {
      fireStateEvent({
        state: State.ERROR,
        message: err,
        details: { error: FlashError.FAILED_FIRMWARE_DOWNLOAD, details: err },
      });
      await esploader.disconnect();
      return;
    }
  }

  fireStateEvent({
    state: State.PREPARING,
    message: "Installation prepared",
    details: { done: true },
  });

  if (eraseFirst) {
    fireStateEvent({
      state: State.ERASING,
      message: "Erasing device...",
      details: { done: false },
    });
    await espStub.eraseFlash();
    fireStateEvent({
      state: State.ERASING,
      message: "Device erased",
      details: { done: true },
    });
  }

  let lastPct = 0;

  fireStateEvent({
    state: State.WRITING,
    message: `Writing progress: ${lastPct}%`,
    details: {
      bytesTotal: totalSize,
      bytesWritten: 0,
      percentage: lastPct,
    },
  });

  let totalWritten = 0;

  for (const part of build.parts) {
    const file = files.shift()!;
    try {
      await espStub.flashData(
        file,
        (bytesWritten: number) => {
          const newPct = Math.floor(
            ((totalWritten + bytesWritten) / totalSize) * 100
          );
          if (newPct === lastPct) {
            return;
          }
          lastPct = newPct;
          fireStateEvent({
            state: State.WRITING,
            message: `Writing progress: ${newPct}%`,
            details: {
              bytesTotal: totalSize,
              bytesWritten: totalWritten + bytesWritten,
              percentage: newPct,
            },
          });
        },
        part.offset,
        true
      );
    } catch (err) {
      fireStateEvent({
        state: State.ERROR,
        message: err,
        details: { error: FlashError.WRITE_FAILED, details: err },
      });
      await esploader.disconnect();
      return;
    }
    totalWritten += file.byteLength;
  }

  fireStateEvent({
    state: State.WRITING,
    message: "Writing complete",
    details: {
      bytesTotal: totalSize,
      bytesWritten: totalWritten,
      percentage: 100,
    },
  });

  await sleep(100);
  await esploader.hardReset();
  await esploader.disconnect();

  fireStateEvent({
    state: State.FINISHED,
    message: "All done!",
  });
};
