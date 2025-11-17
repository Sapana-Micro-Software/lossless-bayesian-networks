#!/bin/bash
# Script to enable GitHub Pages via GitHub CLI
# Requires: GitHub CLI (gh) installed and authenticated

REPO="Sapana-Micro-Software/lossless-bayesian-networks"

echo "🔧 Enabling GitHub Pages for $REPO..."
echo ""

# Check if gh CLI is installed
if ! command -v gh &> /dev/null; then
    echo "❌ GitHub CLI (gh) is not installed."
    echo ""
    echo "Please enable GitHub Pages manually:"
    echo "1. Go to: https://github.com/$REPO/settings/pages"
    echo "2. Under 'Source', select 'GitHub Actions'"
    echo "3. Click 'Save'"
    echo ""
    echo "Or install GitHub CLI:"
    echo "  brew install gh  # macOS"
    echo "  Then run: gh auth login"
    exit 1
fi

# Check if authenticated
if ! gh auth status &> /dev/null; then
    echo "❌ Not authenticated with GitHub CLI."
    echo "Please run: gh auth login"
    exit 1
fi

echo "✅ GitHub CLI is installed and authenticated."
echo ""
echo "Attempting to enable GitHub Pages..."
echo ""

# Enable GitHub Pages via API
# Note: This sets the source to "gh-pages" branch by default
# We need to use the web interface to set it to "GitHub Actions"
RESPONSE=$(gh api repos/$REPO/pages \
    -X POST \
    -f source_type=actions \
    --jq '.html_url' 2>&1)

if [ $? -eq 0 ]; then
    echo "✅ GitHub Pages enabled!"
    echo "📝 Note: You may still need to verify the source is set to 'GitHub Actions'"
    echo "   Check: https://github.com/$REPO/settings/pages"
    echo ""
    echo "🌐 Your site will be available at:"
    echo "   https://sapana-micro-software.github.io/lossless-bayesian-networks/"
else
    echo "⚠️  API call had issues. Please enable manually:"
    echo ""
    echo "1. Go to: https://github.com/$REPO/settings/pages"
    echo "2. Under 'Source', select 'GitHub Actions'"
    echo "3. Click 'Save'"
    echo ""
    echo "Error details:"
    echo "$RESPONSE"
fi
