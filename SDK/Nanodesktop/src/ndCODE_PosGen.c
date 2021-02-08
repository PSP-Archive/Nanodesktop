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
   POSITION GENERATOR (POSGEN)
   ----------------------------------------------------------------------------
   
   Il generatore di posizione (detto anche generatore PosGen) è una particolare
   routine che determina la posizione migliore per una certa finestra di cui
   è nota la lunghezza e l'altezza. 
   
   La posizione migliore viene determinata nel seguente modo: in sostanza, una
   posizione è ritenuta tanto migliore quanto più è ampia la distanza tra le
   altre finestre e tantomeno la finestra copre tutte le altre finestre presenti
   (massimizzate o minimizzate). 
   
   L'algoritmo, in teoria, dovrebbe essere eseguito in questo modo. 
   
   In pratica, prima si impone una posizione teorica per la finestra
   ThX1, ThY1, ThX2, ThY2
   
   Si determina un coefficiente sigma rispetto alla n-esima finestra già trovata,
   che è dato dall'area della n-esima finestra che sarebbe coperta dalla finestra
   nuova se questa fosse posta nelle coordinate (ThX1, ThY1, ThX2, ThY2).
   
   Poi, si determina un coefficiente epsilon che è dato dalla quadrato della
   distanza tra i due centri delle finestre: quella di ordine n e quella teorica.
   
   Alla fine, dunque, si ottengono due coefficienti in funzione di n:
        sigma (n) ed epsilon (n).
        
   Per ciascuna posizione, si sommano tutti i sigma (n) per ogni finestra allocata
   ottenendo il valore sigma_tot.
   
   Poi, si sommano tutti gli epsilon (n) per ogni finestra allocata ottenendo il
   valore epsilon_tot. 
   
   
   Infine, si valutano euristicamente tutte le possibili posizione teoretiche,
   in modo da minimizzare sigma (n) e massimizzare epsilon (n). 
   
   Il vero problema di questo approccio, è che esso richiede un notevole dispendio
   di calcoli. 
   
   Per questo motivo, l'algoritmo prima esegue un passaggio per verificare il
   valore di sigma_tot e epsilon_tot per i soli valori di coordinate che sono
   al centro di un quadrato di 10*10 pixel. 
   
   Successivamente, determinato il quadrato dove più o meno sigma ed epsilon
   sono ottimizzati, si esegue un secondo passaggio per migliorare la precisione
   del risultato, analizzando pixel*pixel.

   -----------------------------------------------------------------------------
*/   

#include <$_NDforND.h>

   
char INTERNAL_TheFirstWindow (unsigned short int LenX,   unsigned short int LenY, 
                              unsigned short int *ResX1, unsigned short int *ResY1,
                              unsigned short int *ResX2, unsigned short int *ResY2)
{
   // Per la prima finestra, si restituisce semplicemente la posizione nell'angolo
   // in alto a sinistra (magari non proprio a margine se possibile....)
  
   if (LenX<Windows_MaxScreenX-10)
   {
      *ResX1 = 10;
      *ResX2 = 10+LenX;
   }
   else
   {
      *ResX1 = 0;
      *ResX2 = LenX;
   }
   
   if (LenY<Windows_MaxScreenY-10)
   {
      *ResY1 = 10;
      *ResY2 = 10+LenY;
   }
   else
   {
      *ResY1 = 0;
      *ResY2 = LenY;
   }
   
   return 0;
}

int INTERNAL_Calculate_Intersect_Len (int Ax1, int Ax2, int Bx1, int Bx2)
{
     // Questa routine ha la funzione di calcolare la lunghezza dell'intersezione
     // tra i due segmenti
     
     if ((Ax2<Bx1) || (Bx2<Ax1))         // I segmenti sono disgiunti: ritorna 0
        return 0;
     else
     {
        if ( (Ax1<=Bx1) && (Ax2>=Bx2) )         // Il segmento A contiene il segmento B
        {
             return (Bx2-Bx1);
        } 
        
        if ( (Bx1<=Ax1) && (Bx2>=Ax2) )         // Il segmento B contiene il segmento A
        {
             return (Ax2-Ax1);
        } 
        
        // Se siamo arrivati fin qui vuol dire che i segmenti non si autocontengono. Allora
        // restano due possibilità:
                   
        if ( (Ax1<=Bx1) && (Bx1<=Ax2) && (Ax2<=Bx2) )
        {
             return (Ax2-Bx1);
        }
        
        if ( (Bx1<=Ax1) && (Ax1<=Bx2) && (Bx2<=Ax2) )
        {
             return (Bx2-Ax1);
        }

        return 0;
     }
}


void INTERNAL_Calculate_Epsilon_Sigma (int *Epsilon, int *Sigma, char WndHandle, int Zx1, int Zy1, int Zx2, int Zy2)
{
   // Iniziamo il calcolo di epsilon.
   
   int Fx1, Fy1, Fx2, Fy2;
   int IntersectionX, IntersectionY;
   
   int CPointX_F, CPointY_F;
   int CPointX_Z, CPointY_Z;
   
   int Diff_CPointX, Diff_CPointY;
   
   Fx1 = WindowData [WndHandle].WndLimitX1;
   Fx2 = WindowData [WndHandle].WndLimitX2;
   Fy1 = WindowData [WndHandle].WndLimitY1;
   Fy2 = WindowData [WndHandle].WndLimitY2;
   
   IntersectionX = INTERNAL_Calculate_Intersect_Len (Fx1, Fx2, Zx1, Zx2);
   IntersectionY = INTERNAL_Calculate_Intersect_Len (Fy1, Fy2, Zy1, Zy2);
   
   *Epsilon = IntersectionX*IntersectionY;
   
   // Ora eseguiamo il calcolo di sigma
   
   CPointX_F = (Fx1+Fx2)/2;
   CPointY_F = (Fy1+Fy2)/2;
   CPointX_Z = (Zx1+Zx2)/2;
   CPointY_Z = (Zy1+Zy2)/2;
   
   Diff_CPointX = abs (CPointX_F - CPointX_Z);
   Diff_CPointY = abs (CPointY_F - CPointY_Z);
   
   *Sigma = ((Diff_CPointX*Diff_CPointX) + (Diff_CPointY*Diff_CPointY)); 
}

void INTERNAL_Calculate_Total_Epsilon_Sigma (ndint64 *TotalEpsilon, ndint64 *TotalSigma, int ThPosX1, int ThPosY1, int ThPosX2, int ThPosY2)
{
    char Counter;
    static int Epsilon;
    static int Sigma;
    
    *TotalEpsilon=0;
    *TotalSigma=0;
    
    for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
    {
       if (WindowData [Counter].WindowIsActive)
       {
           INTERNAL_Calculate_Epsilon_Sigma (&Epsilon, &Sigma, Counter, ThPosX1, ThPosY1, ThPosX2, ThPosY2);
       
           *TotalEpsilon = *TotalEpsilon + Epsilon;
           *TotalSigma   = *TotalSigma + Sigma; 
       }
    } 
}


void INTERNAL_Register_Best_Epsilon_Sigma (struct PosGenData_Type *PosGenData, int ThPosX1, int ThPosY1, int ThPosX2, int ThPosY2, ndint64 TotalEpsilon, ndint64 TotalSigma)
{
      if ( (PosGenData->TheFirstCmp) || (TotalEpsilon < PosGenData->BestEpsilon) || 
            ( (TotalEpsilon==PosGenData->BestEpsilon) && (TotalSigma>PosGenData->BestSigma) ) )
      {
         PosGenData->BestX1 = ThPosX1;
         PosGenData->BestX2 = ThPosX2;
         PosGenData->BestY1 = ThPosY1;
         PosGenData->BestY2 = ThPosY2;
      
         PosGenData->BestSigma   = TotalSigma;
         PosGenData->BestEpsilon = TotalEpsilon;
        
         PosGenData->TheFirstCmp = 0;
      }   
}



void INTERNAL_MakeFirstPassage (struct PosGenData_Type *PosGenData, unsigned short int LenX, unsigned short int LenY)
{
   int CounterX, CounterY;
   
   int Mx1, Mx2, MxC;
   int My1, My2, MyC;
   
   int ThPosX1, ThPosY1, ThPosX2, ThPosY2;
   
   static ndint64 TotalEpsilon;
   static ndint64 TotalSigma;
   
   PosGenData->TheFirstCmp=1;
   
   for (CounterX=LenX; CounterX<Windows_MaxScreenX; CounterX=CounterX+10)
   {
       for (CounterY=LenY; CounterY<Windows_MaxScreenY; CounterY=CounterY+10)
       {        
           Mx1 = CounterX;
           My1 = CounterY;
           
           Mx2 = Mx1 + 10;
           if (Mx2 > Windows_MaxScreenX) Mx2 = Windows_MaxScreenX;
           
           My2 = My1 + 10;
           if (My2 > Windows_MaxScreenY) My2 = Windows_MaxScreenY;
           
           MxC = (Mx1+Mx2)/2;
           MyC = (My1+My2)/2;
           
           // Queste sarebbero le coordinate della finestra se il valore (MxC, MyC)
           // dell'angolo in basso a destra fosse corretto
           
           ThPosX1 = MxC - LenX;
           ThPosY1 = MyC - LenY;
           ThPosX2 = MxC;
           ThPosY2 = MyC;
                   
           INTERNAL_Calculate_Total_Epsilon_Sigma (&TotalEpsilon, &TotalSigma, ThPosX1, ThPosY1, ThPosX2, ThPosY2);                
           INTERNAL_Register_Best_Epsilon_Sigma (PosGenData, ThPosX1, ThPosY1, ThPosX2, ThPosY2, TotalEpsilon, TotalSigma);
           
       }
   }  
}

void INTERNAL_MakeSecondPassage (struct PosGenData_Type *PosGenData, unsigned short int LenX, unsigned short int LenY)
{
   int CounterX, CounterY;
   
   int Sx1, Sx2, Sy1, Sy2;
   
   int ThPosX1, ThPosY1, ThPosX2, ThPosY2;
   
   static ndint64 TotalEpsilon;
   static ndint64 TotalSigma;
   
   // Quando questa routine viene eseguita, in BestX1, BestY1 c'è già una stima della
   // posizione migliore. Dobbiamo migliorare la stima precedente
   
   Sx1 = PosGenData->BestX1-5;
   if (Sx1<0) Sx1=0;
   
   Sx2 = PosGenData->BestX1+5;
   
   if (Sx2+LenX>Windows_MaxScreenX) 
      Sx2 = Windows_MaxScreenX - LenX;
   
   Sy1 = PosGenData->BestY1-5;
   if (Sy1<0) Sy1=0;
   
   Sy2 = PosGenData->BestY1+5;
   
   if (Sy2+LenY>Windows_MaxScreenY) 
      Sy2 = Windows_MaxScreenY - LenY;
   
   PosGenData->TheFirstCmp=1;
   
   for (CounterX=Sx1; CounterX<Sx2; CounterX=CounterX+1)
   {
       for (CounterY=Sy1; CounterY<Sy2; CounterY=CounterY+1)
       {        
           // Calcoliamo le coordinate teoriche
           
           ThPosX1 = CounterX;
           ThPosY1 = CounterY;
           ThPosX2 = CounterX + LenX;
           ThPosY2 = CounterY + LenY;
                   
           INTERNAL_Calculate_Total_Epsilon_Sigma (&TotalEpsilon, &TotalSigma, ThPosX1, ThPosY1, ThPosX2, ThPosY2);                
           INTERNAL_Register_Best_Epsilon_Sigma (PosGenData, ThPosX1, ThPosY1, ThPosX2, ThPosY2, TotalEpsilon, TotalSigma);
           
       }
   }  
}





char ndPOSGEN_GenPosition (unsigned short int LenX,   unsigned short int LenY, 
                           unsigned short int *ResX1, unsigned short int *ResY1,
                           unsigned short int *ResX2, unsigned short int *ResY2)
/*
    Questa routine è il generatore PosGen di Nanodesktop. Trova la posizione
    migliore per una finestra di dimensioni LenX, LenY, e deposita tali
    coordinate nelle variabili puntate da *ResX1, *ResY1, *ResX2, *ResY2

    Se l'operazione va a buon fine restituisce 0, altrimenti restituisce
    l'errore:
    
       ERR_WRONG_PARAMETERS  Le dimensioni della finestra sono troppo grandi
*/
{
   static struct PosGenData_Type PosGenData;   
   
   if ((LenX<Windows_MaxScreenX) && (LenY<Windows_MaxScreenY))
   {
       if (NrWindowsAllocated==0)   // Non sono state ancora allocate finestre
         return INTERNAL_TheFirstWindow (LenX, LenY, ResX1, ResY1, ResX2, ResY2);
       else
       {
         INTERNAL_MakeFirstPassage (&PosGenData, LenX, LenY);
         INTERNAL_MakeSecondPassage (&PosGenData, LenX, LenY);  
         
         // Adesso nelle variabili globali BestX1, BestY1, BestX2 e BestY2 ci sono
         // le coordinate ottimizzate richieste.
         
         *ResX1 = PosGenData.BestX1;
         *ResY1 = PosGenData.BestY1;
         *ResX2 = PosGenData.BestX2;
         *ResY2 = PosGenData.BestY2; 
         
         return 0;
       }   
   }
   else return ERR_WRONG_PARAMETERS; 
}


char ndPOSGEN_ExplodePosition (unsigned short int PosX, unsigned short int PosY, 
                               unsigned short int LenX,   unsigned short int LenY, 
                               unsigned short int *ResX1, unsigned short int *ResY1,
                               unsigned short int *ResX2, unsigned short int *ResY2)
/*
    Questa routine si usa quando c'è una finestra che è stata già allocata, e
    che ha l'angolo in alto a sinistra con coordinate PosX, PosY. 
    
    Se si vuole allocare una nuova finestra con dimensioni LenX e LenY, magari
    maggiori della finestra precedente, questa routine eseguirà i calcoli per
    determinare le nuove coordinate, cercando di non spostare la finestra. 
    
    Se però la finestra andrebbe fuori schermo, le coordinate verranno corrette
    in modo da compensare. 
    
    Le coordinate finali della finestra saranno depositate nelle locazioni di
    memoria puntate da *ResX1, *ResX2, *ResY1, ResY2.
    
       ERR_WRONG_PARAMETERS  Le dimensioni della finestra sono troppo grandi
*/
{
   int ThPosX1, ThPosY1, ThPosX2, ThPosY2;   
   int AdjustX, AdjustY;
   
   if ((LenX<=Windows_MaxScreenX-1) && (LenY<=Windows_MaxScreenY-1))
   {
      ThPosX1 = PosX;
      ThPosY1 = PosY;
      ThPosX2 = PosX + LenX;
      ThPosY2 = PosY + LenY;
      
      AdjustX = Windows_MaxScreenX - 1 - ThPosX2;
      AdjustY = Windows_MaxScreenY - 1 - ThPosY2;
      
      if (AdjustX<0)
      {
          ThPosX1 = ThPosX1 + AdjustX;       // N.B. Equivale a sottrarre una quantità perchè Adjust è negativo
          ThPosX2 = ThPosX2 + AdjustX;
      }
      
      if (AdjustY<0)
      {
          ThPosY1 = ThPosY1 + AdjustY;       // N.B. Equivale a sottrarre una quantità perchè Adjust è negativo
          ThPosY2 = ThPosY2 + AdjustY;
      }
      
      *ResX1 = (unsigned short)(ThPosX1);
      *ResY1 = (unsigned short)(ThPosY1);
      *ResX2 = (unsigned short)(ThPosX2);
      *ResY2 = (unsigned short)(ThPosY2);
   
      return 0;
   }
   else return ERR_WRONG_PARAMETERS; 
}


