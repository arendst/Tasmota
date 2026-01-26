#### Version 2.1.1

- Include all files from `src/fonts` in NPM package.

#### Version 2.1.0

- Icons no longer have zero sidebearings. Each icon is now equal width.
- Minified CSS is no longer generated.
- Updated build process to use `webfonts-generator` to generate all files.
- A JSON dictionary of codepoints is generated for all icons.
- Stylesheets no longer add unnecessary width and height declarations.
- Fix single-quotes @charset declaration in generated CSS file.
- Utility classes have been added for quickly resizing icons, similar to FontAwesome. (-lg, -2x, -3x, etc)
- Added woff2 to generated fonts.

#### Version 2.0.9

- Add source SVG files to `package.json`.

#### Version 2.0.8

- Fix `package.json` files entry for NPM.

#### Version 2.0.7

- Removed SVG ignores from Bower package for developer use.

#### Version 2.0.6

- This release fixes misspelled icons calendar and calendar-outline (Issue #9), renames incorrectly labelled cross to cloud-storage-outline (Issue #5) and adds search terms for many icons to make it easier to find the icon your looking for. It also adds higher decimal rounding for the social-youtube-circular icon to correct some node degradation in the SVG file.

#### Version 2.0.5

- Added non-minified CSS to Bower.

#### Version 2.0.4

- Font files are now licenced using the [SIL Open Font Licence](http://scripts.sil.org/cms/scripts/page.php?item_id=OFL_web). This means that the font can be used, studied, modified and
  redistributed freely as long as they are not sold by themselves.

#### Version 2.0.3

- Added new icons including:
  - Sort down, sort up and unsorted
  - Filter
  - Social: Instagram, YouTube and Google Plus
  - Vendors: Apple, Microsoft and Google
  - Dropbox
  - Half and full hearts
  - Half and full stars
  - Reverse play
  - Thumbs OK
  - CSS3 and HTML5
  - Folder open
  - Mortar board
  - Spiral

#### Version 2.0.2

- Fixed support for IE 8 by replacing double colon with single in `::before`

#### Version 2.0.1

- Added bower support
- Removed EPS for smaller repo size (contact me if you want to get them)

#### Version 2.0.0

- Added 220 new icons, including outline variants
- Improved quality of vector artwork
- Mapped in the Private Use Area of Unicode to avoid being read by screen readers
- Rehinted with `ttfautohint`
- Grouped glyphs on demo page, for convenience
- Changed css glyphs names, to be more semantic
