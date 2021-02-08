all-after:
	
	## First passage
	
	"$(NDENV_PATH)/PSP/SDK/Tools/ndGenCtor/ndGenCtor.exe" --fake
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-gcc" ndpspe_ctor_matrix.c -o ndpspe_ctor_matrix.o $(subst -ansi, ,$(CFLAGS)) -DND_NOT_MAIN_SOURCE 
	
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-ld" ndpspe_ctor_matrix.o $(OBJ) -o "out" --start-group $(subst -mwindows, ,$(subst -c, ,$(LIBS))) -lgcc --end-group -lndSeed_PSPE -lpspaudio -lpspaudiolib -lpspchnnlsv -lpspctrl -lpspctrl_driver -lpspdisplay -lpspdisplay_driver -lpspfpu -lpspgdb -lpspgdb_kernel -lpspgdb_user -lpspge -lpspgu -lpspgum -lpspgum_vfpu -lpsphprm -lpsphprm_driver -lpspkernel -lpspnand_driver -lpspnet -lpspnet_adhoc -lpspnet_adhocctl -lpspnet_adhocmatching -lpspnet_apctl -lpspnet_inet -lpspnet_resolver -lpsppower -lpsprtc -lpspsdk -lpspsircs -lpspumd -lpspusb -lpspusb_driver -lpspusbbus_driver -lpspusbstor -lpspuser -lpsputility -lpspvfpu -lpspwlan -M -Ttext 8900000 > out.map
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/outpatch"

	## Get the table

	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-nm" outp > ndpspe_ctor.map
	
	## Second passage
	
	"$(NDENV_PATH)/PSP/SDK/Tools/ndGenCtor/ndGenCtor.exe"
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-gcc" ndpspe_ctor_matrix.c -o ndpspe_ctor_matrix.o $(subst -ansi, ,$(CFLAGS)) -DND_NOT_MAIN_SOURCE 
		
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/psp-ld" ndpspe_ctor_matrix.o $(OBJ) -o "out" --start-group $(subst -mwindows, ,$(subst -c, ,$(LIBS))) -lgcc --end-group -lndSeed_PSPE -lpspaudio -lpspaudiolib -lpspchnnlsv -lpspctrl -lpspctrl_driver -lpspdisplay -lpspdisplay_driver -lpspfpu -lpspgdb -lpspgdb_kernel -lpspgdb_user -lpspge -lpspgu -lpspgum -lpspgum_vfpu -lpsphprm -lpsphprm_driver -lpspkernel -lpspnand_driver -lpspnet -lpspnet_adhoc -lpspnet_adhocctl -lpspnet_adhocmatching -lpspnet_apctl -lpspnet_inet -lpspnet_resolver -lpsppower -lpsprtc -lpspsdk -lpspsircs -lpspumd -lpspusb -lpspusb_driver -lpspusbbus_driver -lpspusbstor -lpspuser -lpsputility -lpspvfpu -lpspwlan -M -Ttext 8900000 > out.map
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/outpatch"

	## Create the PBP
	
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/elf2pbp" outp $(BIN)
	
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/unpack-pbp" eboot.pbp
	
	cp "$(NDENV_PATH)/PSP/SDK/ServiceFiles/Nanodesktop_PSPE_Cover.png" "./ICON0.PNG"
	
	"$(NDENV_PATH)/PSP/SDK/PspDev/Bin/pack-pbp" EBOOT_PSPE.PBP PARAM.SFO ICON0.PNG ICON1.PMF UNKNOWN.PNG PIC1.PNG SND0.AT3 UNKNOWN.PSP UNKNOWN.PSAR
	
	rm eboot.pbp
	rm PARAM.SFO
	rm ICON0.PNG
	rm ICON1.PMF
	rm UNKNOWN.PNG
	rm PIC1.PNG
	rm SND0.AT3
	rm UNKNOWN.PSP
	rm UNKNOWN.PSAR
	
	xmkdir -p ./pbp_for_pspe
			
	mv EBOOT_PSPE.PBP ./pbp_for_pspe/EBOOT_PSPE.PBP
	
	## Cancella i files temporanei CTOR
	
	rm ndpspe_ctor_matrix.c
	rm ndpspe_ctor_matrix.o
	rm ndpspe_ctor.map

	## Avvia l'emulatore
	
	"$(NDENV_PATH)/PSP/Emulators/PSPE/pspe.exe" ./pbp_for_pspe/EBOOT_PSPE.PBP
	