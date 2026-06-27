#!/usr/bin/env python3
"""
LVGL Header Preprocessor

Extracts function signatures and enums from LVGL header files.
Generates mapping files for Berry scripting integration.
"""

import re
import sys
import glob
import argparse
from pathlib import Path
from typing import List, Set, Tuple, Optional
import logging

import lvgl_config as cfg

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
logger = logging.getLogger(__name__)


class LVGLPreprocessor:
    """Main preprocessor class for LVGL headers."""

    def __init__(self, lv_src_prefix: str = cfg.LV_SRC_PREFIX):
        # All version-sensitive data (exclude lists, header globs, prelude text)
        # lives in lvgl_config.py - the single place to edit on an LVGL bump.
        self.lv_src_prefix = Path(lv_src_prefix)
        self.headers_exclude_suffix = cfg.HEADERS_EXCLUDE_SUFFIX
        self.function_exclude_patterns = cfg.FUNCTION_EXCLUDE_PATTERNS
        self.enum_exclude_prefixes = cfg.ENUM_EXCLUDE_PREFIXES

    def comment_remover(self, text: str) -> str:
        """Remove C/C++ style comments from source code."""
        def replacer(match):
            s = match.group(0)
            return " " if s.startswith('/') else s
        
        pattern = re.compile(
            r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            re.DOTALL | re.MULTILINE
        )
        return re.sub(pattern, replacer, text)

    def list_files(self, prefix: Path, glob_patterns: List[str]) -> List[Path]:
        """Compute a sorted list of files from a prefix and glob patterns."""
        files = []
        for pattern in glob_patterns:
            files.extend(Path(prefix).glob(pattern))
        return sorted(files)

    def clean_source(self, raw: str) -> str:
        """Clean source code by removing comments, preprocessor directives, etc."""
        raw = self.comment_remover(raw)
        
        # Normalize line endings
        raw = re.sub(r'\r\n', '\n', raw)
        raw = re.sub(r'\r', '\n', raw)
        
        # Handle line continuations
        raw = re.sub(r'\\\n', ' ', raw)
        
        # Remove preprocessor directives
        raw = re.sub(r'\n[ \t]*#[^\n]*(?=\n)', '', raw)
        raw = re.sub(r'^[ \t]*#[^\n]*\n', '', raw)
        raw = re.sub(r'\n[ \t]*#[^\n]*$', '', raw)

        # Remove extern "C" blocks
        raw = re.sub(r'extern\s+"C"\s+{(.*)}', r'\1', raw, flags=re.DOTALL)

        # Remove empty lines
        raw = re.sub(r'\n[ \t]*(?=\n)', '', raw)
        raw = re.sub(r'^[ \t]*\n', '', raw)
        raw = re.sub(r'\n[ \t]*$', '', raw)
        
        return raw

    def extract_functions(self, source: str) -> List[str]:
        """Extract function signatures from cleaned source code."""
        # Remove content within braces
        while True:
            source, repl_count = re.subn(r'\{[^{]*?\}', ';', source, flags=re.DOTALL)
            if repl_count == 0:
                break

        # Find function signatures
        pattern = r'(^|;|})\s*([^;{}]+\(.*?\))\s*(?=(;|{))'
        matches = re.findall(pattern, source, flags=re.DOTALL)
        
        functions = []
        for match in matches:
            func_def = match[1]
            # Clean up whitespace
            func_def = re.sub(r'[ \t\r\n]+', ' ', func_def)
            
            # Remove LVGL-specific attributes
            func_def = re.sub(r'LV_ATTRIBUTE_FAST_MEM ', '', func_def)
            func_def = re.sub(r'LV_ATTRIBUTE_TIMER_HANDLER ', '', func_def)
            func_def = re.sub(r'extern ', '', func_def)
            
            # Skip excluded function types
            if any(func_def.startswith(prefix) for prefix in ["typedef", "_LV_", "LV_"]):
                continue
                
            # Extract function name
            name_match = re.search(r'\s(\w+)\([^\(]*$', func_def)
            if not name_match:
                continue
                
            func_name = name_match.group(1)
            
            # Check exclusion patterns
            if any(re.search(pattern, func_name) for pattern in self.function_exclude_patterns):
                continue
                
            functions.append(func_def)
            
        return functions

    def extract_enums(self, source: str) -> Set[str]:
        """Extract enum values from cleaned source code."""
        enum_values = set()
        
        # Find enum definitions
        enum_matches = re.findall(r'enum\s+\w*\s*{(.*?)}', source, flags=re.DOTALL)
        
        for enum_content in enum_matches:
            # Skip LV_PROPERTY_ID enums (disabled feature)
            if 'LV_PROPERTY_ID' in enum_content:
                continue
                
            # Remove macro-defined enums
            enum_content = re.sub(r'\S+\((.*?),.*?\),', r'\1,', enum_content)
            
            # Split by commas and clean up
            for item in enum_content.split(','):
                item = re.sub(r'[ \t\n]', '', item)  # Remove whitespace
                item = re.sub(r'=.*$', '', item)     # Remove assignment
                
                if not item:  # Skip empty items
                    continue
                    
                # Check exclusion patterns
                if any(item.startswith(prefix) for prefix in self.enum_exclude_prefixes):
                    continue
                    
                enum_values.add(item)
        
        # Extract LV_EXPORT_CONST_INT constants
        const_ints = re.findall(r'LV_EXPORT_CONST_INT\((\w+)\)', source, flags=re.DOTALL)
        enum_values.update(const_ints)
        
        return enum_values

    def get_function_headers(self) -> List[Path]:
        """Get list of header files for function extraction."""
        headers = self.list_files(self.lv_src_prefix, cfg.FUNCTION_HEADER_PATTERNS)

        # Add additional headers
        additional_paths = [Path(p) for p in cfg.FUNCTION_HEADER_EXTRA_PATHS]

        for path in additional_paths:
            if path.exists():
                headers.append(path)

        # Filter out excluded files
        return [h for h in headers if not any(str(h).endswith(suffix) for suffix in self.headers_exclude_suffix)]

    def get_enum_headers(self) -> List[Path]:
        """Get list of header files for enum extraction."""
        headers = self.list_files(self.lv_src_prefix, cfg.ENUM_HEADER_PATTERNS)
        return [h for h in headers if not any(str(h).endswith(suffix) for suffix in self.headers_exclude_suffix)]

    def generate_functions_header(self, output_path: Path):
        """Generate the functions header file."""
        logger.info(f"Generating functions header: {output_path}")
        
        headers = self.get_function_headers()
        
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(cfg.FUNCTIONS_HEADER_PRELUDE)
            
            for header_path in headers:
                try:
                    with open(header_path, encoding='utf-8-sig') as header_file:
                        f.write(f"// {header_path}\n")
                        
                        raw_content = self.clean_source(header_file.read())
                        functions = self.extract_functions(raw_content)
                        
                        for func in functions:
                            f.write(f"{func}\n")
                        f.write("\n")
                        
                except Exception as e:
                    logger.error(f"Error processing {header_path}: {e}")

    def generate_enums_header(self, output_path: Path):
        """Generate the enums header file."""
        logger.info(f"Generating enums header: {output_path}")
        
        headers = self.get_enum_headers()
        
        with open(output_path, 'w', encoding='utf-8') as f:
            # Write the static content first
            f.write(self._get_static_enum_content())
            
            # Process headers for dynamic enums
            for header_path in headers:
                try:
                    with open(header_path, encoding='utf-8-sig') as header_file:
                        f.write(f"// File: {header_path}\n")
                        
                        raw_content = self.clean_source(header_file.read())
                        enum_values = self.extract_enums(raw_content)
                        
                        for enum_value in sorted(enum_values):
                            f.write(f"{enum_value}\n")
                        f.write("\n")
                        
                except Exception as e:
                    logger.error(f"Error processing {header_path}: {e}")

    def _get_static_enum_content(self) -> str:
        """Get the static content for enum header."""
        return cfg.ENUM_PRELUDE

    def run(self, functions_output: str = "../mapping/lv_funcs.h", 
            enums_output: str = "../mapping/lv_enum.h"):
        """Run the complete preprocessing pipeline."""
        functions_path = Path(functions_output)
        enums_path = Path(enums_output)
        
        # Create output directories if they don't exist
        functions_path.parent.mkdir(parents=True, exist_ok=True)
        enums_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Generate both files
        self.generate_functions_header(functions_path)
        self.generate_enums_header(enums_path)
        
        logger.info("Preprocessing complete!")


def main():
    """Main entry point with command line argument parsing."""
    parser = argparse.ArgumentParser(description="LVGL Header Preprocessor")
    parser.add_argument("--lv-src", default=cfg.LV_SRC_PREFIX,
                       help="Path to LVGL source directory")
    parser.add_argument("--functions-output", default="../mapping/lv_funcs.h",
                       help="Output path for functions header")
    parser.add_argument("--enums-output", default="../mapping/lv_enum.h", 
                       help="Output path for enums header")
    parser.add_argument("--verbose", "-v", action="store_true",
                       help="Enable verbose logging")
    
    args = parser.parse_args()
    
    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
    
    try:
        preprocessor = LVGLPreprocessor(args.lv_src)
        preprocessor.run(args.functions_output, args.enums_output)
    except Exception as e:
        logger.error(f"Preprocessing failed: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
