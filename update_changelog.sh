#!/bin/bash
changelog mchiasson/GLBrebis all > CHANGELOG.md
git add CHANGELOG.md
git commit -m "Updated changelog"
