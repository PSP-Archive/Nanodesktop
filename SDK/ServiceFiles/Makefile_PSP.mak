
LIBDEV = $(subst -mwindows, ,$(subst -c, ,$(LIBS)))

LIBCFWDEV = "$(NDENV_PATH)\PSP\SDK\HenPspDev\371\lib"



LIBSDK = -lpspdebug -lpspdisplay -lpspgu -lpspge -lpspctrl -lpspsdk -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpsppower -lpsprtc -lpsputility -lpspwlan -lpspaudiolib -lpspaudiocodec -lpspaudio -lpspmpeg -lpspatrac3 -lpspuser -lpspkernel -lgcc -lpspusb_driver -lpspusbbus_driver

LIBHENSDK = -lpspkubridge



XOBJ1 = "$(NDENV_PATH)\PSP\SDK\pspdev\psp\lib\crt0.o"
XOBJ2 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crti.o"
XOBJ3 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crtbegin.o"
XOBJ4 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crtend.o"
XOBJ5 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crtn.o"


LD = "$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-ld"

all-after:
	$(LD) -v $(XOBJ1) $(XOBJ2) $(XOBJ3) -L$(LIBCFWDEV) --start-group $(OBJ) $(LIBDEV) --end-group -lndSeed_PSP $(LIBSDK) $(LIBHENSDK) $(XOBJ4) $(XOBJ5) -o eboot.elf -O -s 
	psp-fixup-imports eboot.elf
	mksfo $(BIN) PARAM.SFO 
	psp-strip eboot.elf -o eboot_strip.elf
	pack-pbp EBOOT.PBP PARAM.SFO "$(NDENV_PATH)/PSP/SDK/ServiceFiles/Nanodesktop_PSP_Cover.png" NULL NULL NULL NULL eboot_strip.elf NULL
	rm eboot.elf
	cp eboot_strip.elf eboot.elf
	rm eboot_strip.elf
	rm *.o
	
	xmkdir -p ./pbp_for_psp
		
	mv eboot.elf ./pbp_for_psp/EBOOT.ELF
	mv eboot.pbp ./pbp_for_psp/EBOOT.PBP




	
