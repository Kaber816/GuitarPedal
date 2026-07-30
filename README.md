# Overview

# BUILD
From source directory run

make -C libDaisy/
make -C DaisySP/ (need to fix unused variables in DaisySP first)
make -C src/

(if you want a compile_commands.json to enable lsp run: "bear -- make -C src/")


