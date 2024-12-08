obj-m += mouse_keyboard_driver.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M="/home/mannantariq/VSCodeProjects/S-5/OS/DeviceDriver/MouseKeyboardDriver" modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M="/home/mannantariq/VSCodeProjects/S-5/OS/DeviceDriver/MouseKeyboardDriver" clean