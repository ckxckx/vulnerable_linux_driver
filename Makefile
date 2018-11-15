obj-m += src/vuln_driver.o

EXTRA_CFLAGS += -fno-stack-protector
all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
clean:
	rm -rf src/*.ko
	rm -rf src/*.mod
	rm -rf src/*.mod.*
	rm -rf src/*.o
	rm -rf .tmp_versions/vuln_driver.mod
	rm -rf src/.vuln_driver.ko.cmd
	rm -rf src/.vuln_driver.mod.o.cmd
	rm -rf src/.vuln_driver.o.cmd
	rm -rf src/*.o.*
	rm -rf Module.symvers
	rm -rf modules.order

