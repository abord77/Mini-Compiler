#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

cs241.binasm < merl.asm > merl.mips
cs241.linkasm < ../!4.mipsSourceCode.asm > output.merl
cs241.linker output.merl print.merl alloc.merl > linked.merl
mips.stdin merl.mips < linked.merl > ../!compiled.mips

rm output.merl
rm linked.merl
rm merl.mips
