# beargit

A simpler version of git, written in C (for educational purposes only).

## Notes

- `tracked-project-example/` is the sample directory that is being tracked
- `build.sh` is a script for compiling the `main.c` which is the main file of the `beargit`
- `src/main.c` is the main file for handling the `beargit` CLI program

## Commands

Currently, `beargit` supports 2 commands:
- `init`: initializes a `beargit` repository locally
- `commit`: automatically commits all the files in the `tracked-project-example` to `.beargit/<commit-hash>`

More will be added soon...

