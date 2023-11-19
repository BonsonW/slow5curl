# Release SOP

1. git checkout main && git pull        # checkout to main and pull
2. git checkout dev && git pull         # checkout to dev and pull
3. git merge main                       # merge main to dev
4. git checkout main && git merge dev   # now merge dev to main
5. update version
    - SLOW5CURL_VERSION in src
    - 2 places in README.md
6. commit the version bump
7. push and wait for all the tests to pass
8. Run test extensive if a major change
9. git tag -a vX.Y.Z -m "version X.Y.Z" # create the tag
10. git push --tags
11. Change the release name to slow5curl-xxx and edit the changes
12. git checkout dev
13. git merge main
14. update to SLOW5CURL_VERSION in src to append -dev
15. commit and push
