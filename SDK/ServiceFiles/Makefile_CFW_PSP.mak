
LIBDEV = $(subst -mwindows, ,$(subst -c, ,$(LIBS)))

LIBCFWDEV = "$(NDENV_PATH)\PSP\SDK\HenPspDev\371\lib"



LIBSDK = -lpspdisplay -lpspgu -lpspge -lpspctrl -lpspsdk \
       -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver \
       -lpsppower -lpsprtc -lpsputility_new -lpspwlan -lpspaudiolib \
       -lpspaudiocodec -lpspaudio -lpspmpeg -lpspatrac3 -lpspuser \
       -lgcc -lpspusb -lpspusbcam -lpspjpeg 
        
LIBHENSDK = -lpspinit -lpspkubridge  -lpspmodulemgr_user  \
	    -lpspsysmem_user -lpspsystemctrl_user \
	    -lpspumd_kernel -lpspusbdevice -lpspusbdevice_driver -lpspvshctrl




LD = "$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-ld"

LDFLAGS = -q -T"$(NDENV_PATH)/PSP/SDK/PspDev/psp/sdk/lib/linkfile.prx"



XOBJ1 = "$(NDENV_PATH)\PSP\SDK\pspdev\psp\lib\crt0.o"
XOBJ2 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crti.o"
XOBJ3 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crtbegin.o"
XOBJ4 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crtend.o"
XOBJ5 = "$(NDENV_PATH)\PSP\SDK\pspdev\lib\gcc\psp\4.0.2\crtn.o"





all-before:
CFLAGS   := $(CFLAGS)    -D"ND_DESIGNED_FOR_CFW"
CXXFLAGS := $(CXXFLAGS)  -D"ND_DESIGNED_FOR_CFW"

all-after:
	$(LD) -v  $(XOBJ1) $(XOBJ2) $(XOBJ3) -L$(LIBCFWDEV) --start-group $(OBJ) $(LIBDEV) --end-group -lndSeed_CFW_PSP $(LIBHENSDK) $(LIBSDK) $(XOBJ4) $(XOBJ5) $(LDFLAGS) -o eboot.elf -O -s -M > out.map 
	psp-fixup-imports eboot.elf
	mksfoex -d MEMSIZE=1 $(BIN) PARAM.SFO 
	psp-strip eboot.elf -o eboot_strip.elf
	psp-prxgen eboot.elf  eboot.prx
	pack-pbp EBOOT.PBP PARAM.SFO "$(NDENV_PATH)/PSP/SDK/ServiceFiles/Nanodesktop_PSPCFW_Cover.png" NULL NULL NULL NULL eboot.prx NULL
	rm eboot.elf
	cp eboot_strip.elf eboot.elf
	rm eboot_strip.elf
	rm *.o
	
	xmkdir -p ./pbp_for_psp_cfw
	
	mv eboot.elf ./pbp_for_psp_cfw/EBOOT.ELF
	mv eboot.prx ./pbp_for_psp_cfw/EBOOT.PRX
	mv eboot.pbp ./pbp_for_psp_cfw/EBOOT.PBP


	
