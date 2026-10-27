# Linux & Git — Command Drill

Fill the last two columns after you run each command in your own terminal.

| Command | What it did | What you saw |
|---|---|---|
| `pwd` | Printed the absolute path of the current directory. | /home/adel |
| `ls` | Listed the files and directories in the current directory. | Downloads  docker_udemy  kubectl |
| `cd` | Changed the working directory to the given path. | No output — the prompt now sits inside /etc (confirm with `pwd`). |
| `mkdir` | Created a new, empty directory. | No output — the new folder appears in `ls`. |
| `cp` | Copied a file to the destination path. | No output — the copy appears in `ls`. |
| `mv` | Moved a file/directory to a new location, or renamed it. | No output — the new name appears in `ls`, the old one is gone. |
| `rm` | Deleted a file. | No output; a confirmation prompt may appear if the file is write-protected. |
| `chmod` | Changed read/write/execute permissions on a file or directory. | No output — verify with `ls -l` (e.g. `rwxr-xr-x` or `755`). |
| `\|` | Piped the stdout of the first command into the stdin of the second. | Only the last command's output is printed (e.g. `ls \| grep txt`). |
| `>` | Redirected stdout into a file, overwriting it or creating it if missing. | No terminal output — the file now contains the result. |
| `>>` | Appended stdout to a file, creating it if missing. | No terminal output — the result is added to the end of the file. |
| `2>&1` | Redirected stderr into the same stream as stdout. | Both normal output and error messages landed in the same file. |
| `grep` | Searched files (or stdin) for lines matching a pattern. | The full matching lines were printed. |
| `git init` | Initialized a new Git repository in the current directory. | `Initialized empty Git repository in /home/adel/project/.git/` |
| `git add` | Staged the latest changes for the next commit. | No output on success. |
| `git commit` | Recorded the staged changes as a commit with a unique hash and message. | `[main 1a2b3c4] message` followed by a summary of files changed. |
| `git status` | Showed the state of the working directory and staging area. | Depends on state: untracked/modified files, or "nothing to commit, working tree clean". |
| `git log` | Listed commits with hash, author, date, and message. | One block per commit: hash, author, date, message. |
| `git diff` | Showed changes between the working directory and the staging area. | Lines prefixed with `+` (added) or `-` (removed). |
| `git push` | Uploaded local commits to the remote repository. | A success summary, or a rejection message. |
| `git pull` | Fetched and merged the latest changes from the remote repository. | A merge summary, or "Already up to date." |
| `git branch` | Listed branches; with arguments, created, deleted, or renamed them. | The branch list, with `*` next to the current branch. |
| `git checkout` | Switched branches (moved HEAD), or restored files. | `Switched to branch 'name'`. |