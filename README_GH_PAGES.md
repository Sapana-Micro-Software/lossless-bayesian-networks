# GitHub Pages Setup

Copyright (C) 2025, Shyamal Chandra

## Overview

This repository includes a complete GitHub Pages site with:

- Modern, responsive design with smooth animations
- Automatic LaTeX documentation compilation
- TypeScript/JavaScript interactivity
- Jekyll-based static site generation
- GitHub Actions workflows for automation

## Structure

```
.
├── _config.yml          # Jekyll configuration
├── index.html           # Main landing page
├── assets/
│   ├── css/
│   │   └── main.css    # Main stylesheet
│   ├── js/
│   │   └── main.js     # Compiled JavaScript
│   └── ts/
│       └── main.ts      # TypeScript source
├── .github/
│   └── workflows/
│       ├── latex-compile.yml  # LaTeX compilation
│       └── pages.yml          # Pages deployment
├── package.json         # Node.js dependencies
├── tsconfig.json        # TypeScript configuration
└── Gemfile             # Ruby/Jekyll dependencies
```

## Setup

### Local Development

1. Install dependencies:
```bash
npm install
bundle install
```

2. Compile TypeScript:
```bash
npm run build
```

3. Serve locally:
```bash
npm run serve
```

### GitHub Pages Deployment

The site is automatically deployed via GitHub Actions when:
- Changes are pushed to the `main` branch
- The workflow is manually triggered

The deployment process:
1. Compiles TypeScript to JavaScript
2. Builds the Jekyll site
3. Deploys to GitHub Pages

## LaTeX Documentation

LaTeX documents are automatically compiled to PDF when `.tex` files change:

- `docs/lossless_bayesian_networks.tex` → `docs/lossless_bayesian_networks.pdf`
- `docs/presentation.tex` → `docs/presentation.pdf`
- `docs/reference_manual.tex` → `docs/reference_manual.pdf`

PDFs are committed back to the repository and linked from the main page.

## Features

- **Responsive Design**: Works on all screen sizes
- **Smooth Animations**: Fade-in and scroll animations
- **Interactive Navigation**: Smooth scrolling and active link highlighting
- **Code Copy**: One-click copy for code blocks
- **Tabbed Interface**: Easy switching between installation methods
- **Modern UI**: Gradient backgrounds, shadows, and hover effects

## Customization

### Colors

Edit CSS variables in `assets/css/main.css`:
```css
:root {
    --primary-color: #6366f1;
    --secondary-color: #8b5cf6;
    /* ... */
}
```

### Content

Edit `index.html` to modify page content and structure.

### TypeScript

Edit `assets/ts/main.ts` and run `npm run build` to compile.

## License

Copyright (C) 2025, Shyamal Chandra
