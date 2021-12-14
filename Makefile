# export PATH=/home/chrissalo/gcc-arm/bin:$PATH

.PHONY: zero1 zero2

all:	zero1 zero2

zero1: 
	$(MAKE) -f Makefile.zero all BUILD_FOR=zero1

zero2:
	$(MAKE) -f Makefile.zero all BUILD_FOR=zero2

clean::
	$(MAKE) -f Makefile.zero clean-all BUILD_FOR=zero1
	$(MAKE) -f Makefile.zero clean BUILD_FOR=zero1
	$(MAKE) -f Makefile.zero clean-all BUILD_FOR=zero2
	$(MAKE) -f Makefile.zero clean BUILD_FOR=zero2
