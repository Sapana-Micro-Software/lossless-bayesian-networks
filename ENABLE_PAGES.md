# Enable GitHub Pages - Quick Guide

## 🚀 Quick Method (Recommended)

### Option 1: Use the Script (if you have GitHub CLI)

```bash
./enable-pages.sh
```

**Prerequisites:**
- Install GitHub CLI: `brew install gh` (macOS) or see [GitHub CLI docs](https://cli.github.com/)
- Authenticate: `gh auth login`

### Option 2: Manual Steps (Always Works)

1. **Open your repository settings:**
   - Go to: https://github.com/Sapana-Micro-Software/lossless-bayesian-networks/settings/pages

2. **Configure the source:**
   - Under **"Source"** section
   - Select **"GitHub Actions"** (NOT "Deploy from a branch")
   - Click **"Save"**

3. **Wait for initialization:**
   - GitHub will take 1-2 minutes to initialize Pages
   - You'll see a green checkmark when ready

4. **Trigger deployment:**
   - Go to: https://github.com/Sapana-Micro-Software/lossless-bayesian-networks/actions
   - Click on **"Deploy GitHub Pages"** workflow
   - Click **"Run workflow"** button
   - Select `main` branch
   - Click **"Run workflow"**

## ✅ Verification

After enabling and deploying, your site will be available at:
- **https://sapana-micro-software.github.io/lossless-bayesian-networks/**

## 🔍 Troubleshooting

### "Source" dropdown is grayed out
- Make sure you have admin/write access to the repository
- Check repository visibility settings

### Can't find "GitHub Actions" option
- Make sure you're looking at the **"Source"** section (not "Custom domain")
- If you only see branch options, the repository might need to be public or you need to upgrade your GitHub plan

### Workflow still fails after enabling
- Wait 2-3 minutes after enabling Pages
- Check that the source is set to "GitHub Actions" (not a branch)
- Re-run the workflow manually from the Actions tab

## 📸 Visual Guide

The settings page should look like this:

```
┌─────────────────────────────────────┐
│ GitHub Pages                        │
├─────────────────────────────────────┤
│ Source                              │
│ ┌─────────────────────────────────┐ │
│ │ GitHub Actions          ▼        │ │ ← Select this
│ └─────────────────────────────────┘ │
│                                      │
│ [Save]                               │
└─────────────────────────────────────┘
```

## 🎯 Next Steps

Once Pages is enabled:
1. ✅ Push any changes to trigger automatic deployment
2. ✅ Or manually run the workflow from Actions tab
3. ✅ Wait 2-5 minutes for deployment to complete
4. ✅ Visit your site URL to verify it's live!
