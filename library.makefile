
.PHONY: install
install:
	mkdir -p $(DESTDIR)$(prefix)include/async
	install --mode=0644 $S/include/async/basic.hpp $(DESTDIR)$(prefix)include/async/
	install --mode=0644 $S/include/async/result.hpp $(DESTDIR)$(prefix)include/async/
	install --mode=0644 $S/include/async/jump.hpp $(DESTDIR)$(prefix)include/async/
