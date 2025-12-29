# Animation DSL for VSCode

This extension provides syntax highlighting, snippets, and language support for the Berry Animation Framework DSL (.anim files).

## Features

### Syntax Highlighting
- **Keywords**: `strip`, `color`, `palette`, `animation`, `sequence`, `template`, `import`, `set`, `play`, `run`, `if`, `repeat`, `reset`, `restart`, `log`, etc.
- **Animation Functions**: `solid`, `pulsating_animation`, `rich_palette_animation`, `beacon_animation`, `comet_animation`, etc.
- **Value Providers**: `triangle`, `cosine_osc`, `sawtooth`, `color_cycle`, `strip_length`, `ramp`, `linear`, `smooth`, `square`
- **Mathematical Functions**: `abs`, `max`, `min`, `round`, `sqrt`, `scale`, `sin`, `cos`
- **User Functions**: `user.function_name()` syntax with proper highlighting
- **Colors**: Hex colors (`#FF0000`, `0xFF0000`, `0x80FF0000`) and named colors (red, blue, etc.)
- **Time Literals**: 2s, 500ms, 1m, 2h
- **Percentages**: 50%, 100%
- **Comments**: Line comments starting with #
- **Template Syntax**: Template definitions with parameter type annotations

### Language Features
- **Auto-closing**: Brackets, quotes, and braces
- **Comment toggling**: Ctrl+/ for line comments
- **Bracket matching**: Matching brackets and braces
- **Indentation**: Smart indentation for sequences and loops
- **Theme Compatibility**: Works with any VSCode theme (dark, light, high contrast)

## File Association

This extension automatically activates for files with the `.anim` extension.

## Theme Compatibility

The extension uses semantic token scopes that work with any VSCode theme:
- **Keywords**: Uses standard keyword colors from your chosen theme
- **Functions**: Uses function colors from your chosen theme
- **Constants**: Uses constant colors from your chosen theme (colors, numbers, etc.)
- **Comments**: Uses comment colors from your chosen theme
- **Strings**: Uses string colors from your chosen theme

This ensures the syntax highlighting looks great whether you prefer dark themes, light themes, or high contrast themes.

## Installation

### From VSIX (Recommended)
1. Download the `.vsix` file
2. Open VSCode
3. Go to Extensions (Ctrl+Shift+X)
4. Click the "..." menu and select "Install from VSIX..."
5. Select the downloaded `.vsix` file

### Manual Installation
1. Copy the extension folder to your VSCode extensions directory:
   - **Windows**: `%USERPROFILE%\.vscode\extensions\`
   - **macOS**: `~/.vscode/extensions/`
   - **Linux**: `~/.vscode/extensions/`
2. Restart VSCode

## Development

### Building the Extension
```bash
npm install -g @vscode/vsce
cd vscode-animation-dsl
vsce package
```

This creates a `.vsix` file that can be installed in VSCode.

### Testing
1. Open the extension folder in VSCode
2. Press F5 to launch a new Extension Development Host
3. Open a `.anim` file to test syntax highlighting

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

### Adding New Features
- **Keywords**: Add to `syntaxes/animation-dsl.tmLanguage.json`
- **Token Scopes**: Modify semantic scopes in the grammar file

## License

This extension is part of the Berry Animation Framework project and follows the same license terms.

## Related

- [Berry Animation Framework](https://github.com/tasmota/berry-animation-framework)
- [Tasmota](https://tasmota.github.io/docs/)
- [Berry Language](https://github.com/berry-lang/berry)