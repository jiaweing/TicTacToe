export COMMIT_HASH=$(git rev-parse --short "$GITHUB_SHA")
echo "COMMIT_HASH=$COMMIT_HASH" >> $GITHUB_ENV