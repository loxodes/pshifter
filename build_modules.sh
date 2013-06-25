# ... change this to a loop

cp ~/nshifter/software/process/pshifter_config.h.e pshifter_config.h
make
mv nshifter.elf e.elf

cp ~/nshifter/software/process/pshifter_config.h.f pshifter_config.h
make
mv nshifter.elf f.elf

cp ~/nshifter/software/process/pshifter_config.h.g pshifter_config.h
make
mv nshifter.elf g.elf

cp ~/nshifter/software/process/pshifter_config.h.h pshifter_config.h
make
mv nshifter.elf h.elf



