/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ----------------------------------------------------------------------------
    ROUTINES PER LA GESTIONE DEL SERVICE SPACE
    ----------------------------------------------------------------------------
    Il service space è uno speciale spazio di servizio che viene usato per
    gestire i menu di finestra. 
    
    Le routine del service space sono tutte interne, non essendo consentito alle
    applicazioni utente di scrivere in questo spazio
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

inline char INTERNAL_ndSS_IsPixelInSS (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/*  
    Questa routine restituisce il valore 1 se il pixel è nell'ambito del service space,
    altrimenti restituisce 0.
*/
{
    return  ( (RRPosPixelX <= WindowData [WndHandle].SS_LenX) &&
              (RRPosPixelY <= WindowData [WndHandle].SS_LenY)  ); 
}


inline void ndSS_PutPixel_NORENDER_FAST (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, TypeColor Color, unsigned char WndHandle)
{
   WINDOWDATA_SS (WndHandle, RRPosPixelX, RRPosPixelY)=Color;               
   // Inserisce il colore nel service space    
}


char ndSS_PutPixel (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione scrive un pixel nel service space. 
   
   La routine restituisce i seguenti codici di errore:
     ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è stata allocata
     ERR_PIXEL_NOT_IN_SS           Il pixel non è nel service space
     ERR_SS_NOT_SUPPORTED          Questa finestra non supporta i service space
*/

{
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].SS_Supported)
      {
          if (INTERNAL_ndSS_IsPixelInSS (RRPosPixelX, RRPosPixelY, WndHandle))
          {
              ndSS_PutPixel_NORENDER_FAST (RRPosPixelX, RRPosPixelY, Color, WndHandle);
              if (RenderNow) XWindowRender (WndHandle);
              
              return 0;
          }
          else
            return ERR_PIXEL_NOT_IN_SS;
      }
      else
        return ERR_SS_NOT_SUPPORTED;
   }
   else
     return ERR_WINDOW_IS_NOT_ALLOCATED;
     
}



char ndSS_DrawRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, unsigned short int RRPosX2, unsigned short int RRPosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nel service space della finestra identificata da WndHandle.
     Al termine dell'operazione viene eseguito il rendering.

     Sono previsti due codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
          ERR_RECTANGLE_OUT_OF_SS              Il rettangolo andrebbe fuori dai limiti della service space
          ERR_SS_NOT_SUPPORTED                 Questa finestra non supporta i service space
*/
{
    if ( !ndInitSystemExecuted ) ndInitSystem();
          
	if (WindowData [WndHandle].WindowIsActive)
	{
      if (WindowData [WndHandle].SS_Supported)
      {
         int IntX, IntY;
	     unsigned short int KDiffX, KDiffY;

		 if (RRPosX1>RRPosX2) UShort_Swap (&RRPosX1, &RRPosX2);		
		 if (RRPosY1>RRPosY2) UShort_Swap (&RRPosY1, &RRPosY2);
		
		 KDiffX = RRPosX2 - RRPosX1;
		 KDiffY = RRPosY2 - RRPosY1;
        
         if ( (KDiffX<WindowData [WndHandle].SS_LenX) && (KDiffY<WindowData [WndHandle].SS_LenY) )
         {
         	//Disegniamo prima l'interno del rettangolo

            for (IntY=0; IntY<=KDiffY; IntY++)
		    {
			    for (IntX=0; IntX<=KDiffX; IntX++)
				{
					ndSS_PutPixel_NORENDER_FAST (IntX+RRPosX1, IntY+RRPosY1, Color, WndHandle);
		        }
            }

	        //Ora disegniamo il border del rettangolo: lato superiore
	        //ed inferiore

	        for (IntX=0; IntX<=KDiffX; IntX++)
		    {
			    ndSS_PutPixel_NORENDER_FAST (IntX+RRPosX1, RRPosY1, BorderColor, WndHandle);
			    ndSS_PutPixel_NORENDER_FAST (IntX+RRPosX1, RRPosY2, BorderColor, WndHandle);
		    }

            //Ora disegniamo il border del rettangolo: lato destro e
	        //e sinistro

	        for (IntY=0; IntY<=KDiffY; IntY++)
			{
			    ndSS_PutPixel_NORENDER_FAST (RRPosX1, IntY+RRPosY1, BorderColor, WndHandle);
			    ndSS_PutPixel_NORENDER_FAST (RRPosX2, IntY+RRPosY1, BorderColor, WndHandle);
            }

            // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
            // rettangolo

            if (RenderNow) XWindowRender (WndHandle);
            return 0;
          }
          else
            return ERR_RECTANGLE_OUT_OF_SS;
       }
       else
         return ERR_SS_NOT_SUPPORTED;
    }
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;

}


char ndSS_DrawRectangleVoid (unsigned short int RRPosX1, unsigned short int RRPosY1, unsigned short int RRPosX2, unsigned short int RRPosY2, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nel service space della finestra identificata da WndHandle.
     Al termine dell'operazione viene eseguito il rendering.

     Sono previsti due codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
          ERR_RECTANGLE_OUT_OF_SS              Il rettangolo andrebbe fuori dai limiti della SS
          ERR_SS_NOT_SUPPORTED                 Questa finestra non supporta i service space
*/
{
    if ( !ndInitSystemExecuted ) ndInitSystem();
          
	if (WindowData [WndHandle].WindowIsActive)
	{
      if (WindowData [WndHandle].SS_Supported)
      {
         int IntX, IntY;
	     unsigned short int KDiffX, KDiffY;

		 if (RRPosX1>RRPosX2) UShort_Swap (&RRPosX1, &RRPosX2);		
		 if (RRPosY1>RRPosY2) UShort_Swap (&RRPosY1, &RRPosY2);
		
		 KDiffX=RRPosX2-RRPosX1;
         KDiffY=RRPosY2-RRPosY1;
  		
         if ( (KDiffX<WindowData [WndHandle].SS_LenX) && (KDiffY<WindowData [WndHandle].SS_LenY) )
         {
	        //Ora disegniamo il border del rettangolo: lato superiore
	        //ed inferiore

	        for (IntX=0; IntX<=KDiffX; IntX++)
		    {
			    ndSS_PutPixel_NORENDER_FAST (IntX+RRPosX1, RRPosY1, BorderColor, WndHandle);
			    ndSS_PutPixel_NORENDER_FAST (IntX+RRPosX1, RRPosY2, BorderColor, WndHandle);
		    }

            //Ora disegniamo il border del rettangolo: lato destro e
	        //e sinistro

	        for (IntY=0; IntY<=KDiffY; IntY++)
			{
			    ndSS_PutPixel_NORENDER_FAST (RRPosX1, IntY+RRPosY1, BorderColor, WndHandle);
			    ndSS_PutPixel_NORENDER_FAST (RRPosX2, IntY+RRPosY1, BorderColor, WndHandle);
            }

            // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
            // rettangolo

            if (RenderNow) XWindowRender (WndHandle);
            return 0;
          }
          else
            return ERR_RECTANGLE_OUT_OF_SS;
       }
       else
        return ERR_SS_NOT_SUPPORTED;
    }
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;

}


char ndSS_DrawRectangleRounded (unsigned short int RRPosX1, unsigned short int RRPosY1, unsigned short int RRPosX2, unsigned short int RRPosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo dai contorni arrotondati nel service space della finestra 
     identificata da WndHandle.
     Al termine dell'operazione viene eseguito il rendering.

     Sono previsti due codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
          ERR_RECTANGLE_OUT_OF_SS              Il rettangolo andrebbe fuori dai limiti del service space
          ERR_SS_NOT_SUPPORTED                 Questa finestra non supporta i service space
*/
{      
  if ( !ndInitSystemExecuted ) ndInitSystem();
          
  if (WindowData [WndHandle].WindowIsActive)
  {
    if (WindowData [WndHandle].SS_Supported)
    {                   
        int IntX, IntY;
	    unsigned short int KDiffX, KDiffY;

        char Offset   [10] = {2, 1, 0, 0, 0, 0, 0, 0, 0};      
        char NrPixels [10] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned short int PosOff, OffsetChoose, NrPixelsChoose;
        
        
		if (RRPosX1>RRPosX2) UShort_Swap (&RRPosX1, &RRPosX2);
        if (RRPosY1>RRPosY2) UShort_Swap (&RRPosY1, &RRPosY2);
		
		KDiffX=RRPosX2-RRPosX1;
        KDiffY=RRPosY2-RRPosY1;
  		
        if ( (KDiffX<WindowData [WndHandle].SS_LenX) && (KDiffY<WindowData [WndHandle].SS_LenY) )
         {
            for (IntY=0; IntY<=KDiffY; IntY++)
            {
                if ( (IntY>=0) && (IntY<=8) )
                {
                   PosOff         = IntY;
                   OffsetChoose   = Offset   [PosOff];
                   NrPixelsChoose = NrPixels [PosOff]-1;      
                }
                
                if ( (IntY>=KDiffY-8) && (IntY<=KDiffY) )
                {
                   PosOff         = KDiffY - IntY;
                   OffsetChoose   = Offset [PosOff];
                   NrPixelsChoose = NrPixels [PosOff]-1;
                }
                
                if ( (IntY>8) && (IntY<KDiffY-8) )
                {
                   OffsetChoose   = 0;
                   NrPixelsChoose = 0;
                }
         
                if ((IntY==0) || (IntY==KDiffY))      // E' la prima o l'ultima riga del rettangolo
                {
                   for (IntX=OffsetChoose; IntX<=KDiffX-OffsetChoose; IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                }
                else  // La riga è diversa dalla prima o dall'ultima
                {
                   for (IntX=OffsetChoose; IntX<=OffsetChoose + NrPixelsChoose; IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                   
                   for (IntX=(OffsetChoose + NrPixelsChoose + 1); IntX<=(KDiffX - NrPixelsChoose - OffsetChoose - 1); IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, Color, WndHandle);
                   
                   for (IntX=(KDiffX - NrPixelsChoose - OffsetChoose); IntX<=(KDiffX - OffsetChoose); IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                }   
                   
            }
            
            // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
            // rettangolo

            if (RenderNow) XWindowRender (WndHandle);
            return 0;
          }
          else
            return ERR_RECTANGLE_OUT_OF_SS;
     }
     else
      return ERR_SS_NOT_SUPPORTED;
 }
 else
   return ERR_WINDOW_IS_NOT_ALLOCATED;

}


char ndSS_DrawRectangleRoundedVoid (unsigned short int RRPosX1, unsigned short int RRPosY1, unsigned short int RRPosX2, unsigned short int RRPosY2, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo dai contorni arrotondati e totalmente vuoto al suo interno
     nel service space della finestra identificata da WndHandle.
     Al termine dell'operazione viene eseguito il rendering.

     Sono previsti due codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
          ERR_RECTANGLE_OUT_OF_SS             Il rettangolo andrebbe fuori dai limiti del service space
          ERR_SS_NOT_SUPPORTED                Questa finestra non supporta i service space

*/
{
    if ( !ndInitSystemExecuted ) ndInitSystem();
          
	if (WindowData [WndHandle].WindowIsActive)
	{
      if (WindowData [WndHandle].SS_Supported)
      {            
                                
        int IntX, IntY;
	    unsigned short int RRPosX1, RRPosX2, RRPosY1, RRPosY2, KDiffX, KDiffY;

        char Offset   [10] = {2, 1, 0, 0, 0, 0, 0, 0, 0};      
        char NrPixels [10] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned short int PosOff, OffsetChoose, NrPixelsChoose;
        
		if (RRPosX1>RRPosX2) UShort_Swap (&RRPosX1, &RRPosX2);
		if (RRPosY1>RRPosY2) UShort_Swap (&RRPosY1, &RRPosY2);
		
		KDiffX=RRPosX2-RRPosX1;
        KDiffY=RRPosY2-RRPosY1;
  		
        if ( (KDiffX<WindowData [WndHandle].SS_LenX) && (KDiffY<WindowData [WndHandle].SS_LenY) )
         {
            for (IntY=0; IntY<=KDiffY; IntY++)
            {
                if ( (IntY>=0) && (IntY<=8) )
                {
                   PosOff         = IntY;
                   OffsetChoose   = Offset   [PosOff];
                   NrPixelsChoose = NrPixels [PosOff]-1;      
                }
                
                if ( (IntY>=KDiffY-8) && (IntY<=KDiffY) )
                {
                   PosOff         = KDiffY - IntY;
                   OffsetChoose   = Offset [PosOff];
                   NrPixelsChoose = NrPixels [PosOff]-1;
                }
                
                if ( (IntY>8) && (IntY<KDiffY-8) )
                {
                   OffsetChoose   = 0;
                   NrPixelsChoose = 0;
                }
         
                if ((IntY==0) || (IntY==KDiffY))      // E' la prima o l'ultima riga del rettangolo
                {
                   for (IntX=OffsetChoose; IntX<=KDiffX-OffsetChoose; IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                }
                else  // La riga è diversa dalla prima o dall'ultima
                {
                   for (IntX=OffsetChoose; IntX<=OffsetChoose + NrPixelsChoose; IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                   
                   for (IntX=(KDiffX - NrPixelsChoose - OffsetChoose); IntX<=(KDiffX - OffsetChoose); IntX++)
                       ndSS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                }   
                   
            }
            
            // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
            // rettangolo

            if (RenderNow) XWindowRender (WndHandle);
            return 0;
          }
          else
            return ERR_RECTANGLE_OUT_OF_SS;
       }
       else
        return ERR_SS_NOT_SUPPORTED;
    }
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;          
}






char ndSS_DrawLine      (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                         unsigned short int RRPosX2, unsigned short int RRPosY2, 
                         TypeColor Color, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive una linea sullo schermo, tra i due punti le cui coordinate sono
     date da RRPosX1,RRPosY1 e RRPosX2,RRPosY2. 
     
     E' previsto due codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
          ERR_SS_NOT_SUPPORTED                La finestra non supporta service space
   
*/

{
  float CounterR;
  float ScaleX, ScaleY;
  unsigned short int KPosX, KPosY;
       
  if ( !ndInitSystemExecuted ) ndInitSystem();
       
       
  if (WindowData [WndHandle].WindowIsActive)
  {          
    if (WindowData [WndHandle].SS_Supported)
    {
       CounterR=0;
       
       ScaleX = RRPosX2 - RRPosX1;
       ScaleY = RRPosY2 - RRPosY1;
     
       for (CounterR=0; CounterR<1; CounterR=CounterR+0.005)
       {     
           KPosX = (unsigned short int)((int)RRPosX1 + (int)(ScaleX * CounterR) );
           KPosY = (unsigned short int)((int)RRPosY1 + (int)(ScaleY * CounterR) );
           
           ndSS_PutPixel (KPosX, KPosY, Color, WndHandle, NORENDER);
       }
              
       if (RenderNow) XWindowRender (WndHandle);
       return 0;
     }
     else
       return ERR_SS_NOT_SUPPORTED;
  }
  else
    return ERR_WINDOW_IS_NOT_ALLOCATED;
    
}



char ndSS_ClrBufferSpace (unsigned char WndHandle, char RenderNow)
/*
     Questa funzione pulisce il buffer SS della finestra identificata da WndHandle.
     Al termine dell'operazione viene eseguita una operazione di X-Render per mostrare il risultato.

     La funzione restituisce i codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
          ERR_SS_NOT_SUPPORTED                La finestra non supporta il service space
*/

{
  if ( !ndInitSystemExecuted ) ndInitSystem();
          
  unsigned short int CounterX, CounterY;

  if (WindowData [WndHandle].WindowIsActive)
  {
     if (WindowData [WndHandle].SS_Supported)
     {
        for (CounterY=0; CounterY<(WindowData [WndHandle].SS_LenY); CounterY++)
        {
            for (CounterX=0; CounterX<(WindowData [WndHandle].SS_LenX); CounterX++)
            {
                ndSS_PutPixel_NORENDER_FAST (CounterX, CounterY, 0, WndHandle);
            }
        }

        if (RenderNow) XWindowRender (WndHandle);
        return 0;
     }
     else
       return ERR_SS_NOT_SUPPORTED; 
  }
  else
    return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndSS_WriteCharInSS (unsigned int RRPosPixelX, unsigned int RRPosPixelY, char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione permette di scrivere un carattere nel service space della finestra indicata da WndHandle.
     Il carattere viene scritto come se si trattasse di un semplice elemento grafico: il service space non
     supporta un cursore.
                  
     Le coordinate devono essere espresse rispetto al punto più in alto ed a sinistra del service space.
      
     Questa funzione è stata progettata per la scrittura di caratteri singoli. Al termine del processo, richiama
     XWindowRender per disegnare il carattere sullo schermo.

     Se non si vuole che questa avvenga, si può passare come ultimo parametro la costante manifesta NORENDER
     (che è comunemente usato da funzioni come ndSS_WriteLetter o ndSS_WriteLn).

     Questa funzione restituisce i codici di errore:

           ERR_WINDOW_IS_NOT_ALLOCATED
           ERR_SS_NOT_SUPPORTED
*/

{
  unsigned char *FontData;		//puntatore al font

  unsigned char BitFilter, BitExtract;
  unsigned char ByteInChar;
  unsigned char FntHandle;
  int  IntX, IntY;
    
  char ErrRep;
  
  if ( !ndInitSystemExecuted ) ndInitSystem();
           
  if ((WindowData [WndHandle].WindowIsActive))
  {
     if (WindowData [WndHandle].SS_Supported)
     {
	    if (Carattere!=0)
	    {
		   FntHandle=WindowData [WndHandle].ndMB_CurrentFntHandle;  
           FontData=FontHandlesArray [FntHandle].Data+Carattere*8;		                                                
           //Inizializza il puntatore alla corretta lettera nel buffer dei font

		   ByteInChar=0;

		   for (IntY=0; IntY<8; IntY++)
		   {
		   	   BitFilter=0x80;

			   for (IntX=0; IntX<8; IntX++)
			   {
				   BitExtract=(*(FontData+ByteInChar))&BitFilter;

				   if (BitExtract!=0)
                      ErrRep=ndSS_PutPixel (RRPosPixelX+IntX, RRPosPixelY+IntY, Color, WndHandle, NORENDER);
                   else
                      ErrRep=ndSS_PutPixel (RRPosPixelX+IntX, RRPosPixelY+IntY, BGColor, WndHandle, NORENDER);

                   if (ErrRep!=0) return ErrRep;

				   BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
			    }

			    ByteInChar++;

           }

           if (RenderNow) XWindowRender (WndHandle);           
        }
        
        return 0;
     }
     else
       return ERR_SS_NOT_SUPPORTED;
  }
  else
    return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndSS_WriteStringInSS (unsigned short RRPosPixelX, unsigned short int RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione serve per scrivere una stringa nel service space di una finestra identificata da
   WndHandle. La stringa viene scritta come un semplice elemento grafico.
   
   La stringa comparirà nel service space a partire dal pixel determinato dalle coordinate RPosPixelX
   e RPosPixelY. Tali coordinate devono essere fornite rispetto al punto in alto a sinistra del
   service space. 
  
   Anche per questa funzione sono previsti i codici di errore di uscita:

         ERR_WINDOW_IS_NOT_ALLOCATED
         ERR_SS_NOT_SUPPORTED
*/

{
  int InternalPosX, InternalPosY;
         
  int MaxLenStr=255;
  int t=0;
  char *strpointer;			//Puntatore alla stringa

  if ( !ndInitSystemExecuted ) ndInitSystem();

  if (WindowData [WndHandle].WindowIsActive)
  {
     if (WindowData [WndHandle].SS_Supported)
     {
    	strpointer=str;

	    while (*strpointer!=0 && t<MaxLenStr)
	    {
              InternalPosX=RRPosPixelX + t*8;
              InternalPosY=RRPosPixelY;
              
		      ndSS_WriteCharInSS (InternalPosX, InternalPosY, *strpointer, Color, BGColor, WndHandle, NORENDER);
		      strpointer++;

              t++;
        }

        
        if (RenderNow) XWindowRender (WndHandle);          // Al termine del processo di copia il rendering fa apparire la stringa
        return 0;
     
     }
     else
       return ERR_SS_NOT_SUPPORTED;
 }
 else
   return ERR_WINDOW_IS_NOT_ALLOCATED;

}


char ndSS_EnableServiceSpace (unsigned char WndHandle)
{
    if (WindowData [WndHandle].WindowIsActive)
    {
         if (WindowData [WndHandle].SS_Supported)
         {
             WindowData [WndHandle].SS_Enabled = 1; 
             return 0;          
         }
         else
           return ERR_SS_NOT_SUPPORTED;
    } 
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndSS_DisableServiceSpace (unsigned char WndHandle)
{
    if (WindowData [WndHandle].WindowIsActive)
    {
         if (WindowData [WndHandle].SS_Supported)
         {
             WindowData [WndHandle].SS_Enabled = 0;           
             return 0;
         }
         else
           return ERR_SS_NOT_SUPPORTED;
    } 
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;
}





