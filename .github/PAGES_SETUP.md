# GitHub Pages Setup Guide

## Enable GitHub Pages

Before the deployment workflow can run, you need to enable GitHub Pages in your repository settings:

1. Go to: https://github.com/Sapana-Micro-Software/lossless-bayesian-networks/settings/pages
2. Under "Source", select **"GitHub Actions"** (not "Deploy from a branch")
3. Save the settings

## Verify Workflow Permissions

The workflow requires the following permissions:
- `contents: read` - To read repository files
- `pages: write` - To deploy to GitHub Pages
- `id-token: write` - For OIDC authentication

These are already configured in the workflow file.

## Troubleshooting

### Error: "Failed to create deployment (status: 404)"
- **Solution**: Make sure GitHub Pages is enabled and set to use "GitHub Actions" as the source

### Error: "Cannot find any run with github.run_id"
- **Solution**: This usually means the workflow needs to be re-run after enabling GitHub Pages
- Try manually triggering the workflow from the Actions tab

### Workflow Not Running
- Check that you've pushed changes to the `main` branch
- Verify the workflow file is in `.github/workflows/pages.yml`
- Check the Actions tab for any error messages

## Manual Deployment

If you need to manually trigger the deployment:

1. Go to the Actions tab in your repository
2. Select "Deploy GitHub Pages" workflow
3. Click "Run workflow" button
4. Select the branch (usually `main`)
5. Click "Run workflow"

## Testing Locally

Before deploying, you can test the site locally:

```bash
# Install dependencies
npm install
bundle install

# Compile TypeScript
npm run build

# Build Jekyll site
bundle exec jekyll build --baseurl /lossless-bayesian-networks

# Serve locally
bundle exec jekyll serve
```

Then visit: http://localhost:4000/lossless-bayesian-networks/
