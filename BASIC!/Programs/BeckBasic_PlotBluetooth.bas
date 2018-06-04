PRINT "BeckBasic_PlotBluetooth.bas,6/3/18a"

flagOri= 1		%Portrait
GOSUB openScreen
GOSUB userfunctions
CALL SetupPlot()

! Begin by opening Bluetooth
! If Bluetooth is not enabled
! the program will stop here.
BT.OPEN
PAUSE 500	%Need pause because we're in graphics mode

! When BT is opened, the program
! will start listening for another
! device to connect. At this time
! the user can continue to wait
! for a connection are can attempt
! to connect to another device
! that is waiting for a connection

! Ask user what to do
ARRAY.LOAD type$[], "Connect to listener", "Continue to listen for connection", "Quit"
title$ = "Select operation mode"

! Create the menu that will be loaded
! when the screen is touched.
ARRAY.LOAD menu$[], "Send", "Disconnect","Quit"

new_connection:
xdomenu =0
SELECT type, type$[], title$

! If the user pressed the back
! key or selected quit then quit
! otherwise try to connect to
! a listener
IF (type = 0) | (type =3)
 PRINT "See you later"
 BT.CLOSE
 END
ELSEIF type = 1
 BT.CONNECT
ENDIF

! Read status until a connection is made
ln = 0
DO
 BT.STATUS s
 IF s = 1
  ln = ln + 1
  PRINT "Listening", ln
 ELSEIF s =2
  PRINT "Connecting"
 ELSEIF s = 3
  PRINT "Connected"
  PRINT "Touch any text line to send, disconnect or quit."
 ENDIF		%IF s=1
 PAUSE 1000
UNTIL s = 3

! When a connection is made
! get the name of the connected
! device
BT.DEVICE.NAME device$

! *** Read/Write Loop ****
RW_Loop:
DO
 ! If the screen is touched, the interrupt
 ! code will change xdoMemu to 1 (true)
 ! In that case, show the menu
 IF xdoMenu
  xdoMenu =0
  SELECT menu,  menu$[], "Do what?"
  IF menu = 1 THEN GOSUB xdoSend
  IF menu = 2 THEN BT.DISCONNECT
  IF menu = 3
   PRINT "Bye bye"
   BT.CLOSE
   END
  ENDIF	  %IF menu=3
 ENDIF	%IF xdoMenu

 ! Read status to insure
 ! that we remain connected.
 ! If disconnected, program
 ! reverts to listen mode.
 ! In that case, ask user
 ! what to do.

 BT.STATUS s
 IF s<> 3
  PRINT "Connection lost"
  GOTO new_connection
 ENDIF		%IF s<>3

 ! Read messages until
 ! the message queue is
 ! empty
 DO
  BT.READ.READY rr
  IF rr
   BT.READ.BYTES rmsg$
   !PRINT device$;": ";rmsg$
   PlotDataValue(device$, rmsg$)
  ENDIF		%IF rr
 UNTIL (rr = 0)		%BT.READ.READY
UNTIL 0		%RW_Loop

! Get and send message
! to the connected device
xdoSend:
	INPUT "Text to send", wmsg$
	BT.WRITE wmsg$
	PRINT "Me: "; wmsg$
RETURN

! When Console is touched
! set xdoMenu to true
onConsoleTouch:
	xdoMenu = 1
ConsoleTouch.Resume

!*************************************************************8
! Code fromBeckBasic_PlotFunc.bas,6/3/18m

!--------------------------------------------
userfunctions:
FN.DEF	SetupPlot()
! create a diagram bundle (...object) -------
BUNDLE.CREATE     diag1

BUNDLE.PUT        diag1, "npoints"      ,  ctr
BUNDLE.PUT        diag1, "xs"           , -_2pi
BUNDLE.PUT        diag1, "xe"           ,  _2pi
BUNDLE.PUT        diag1, "ys"           ,  -1.1
BUNDLE.PUT        diag1, "ye"           ,  1.1
BUNDLE.PUT        diag1, "posX1"        ,  0.05
BUNDLE.PUT        diag1, "posY1"        ,  0.05
BUNDLE.PUT        diag1, "posX2"        ,  0.95
BUNDLE.PUT        diag1, "posY2"        ,  0.3
!BUNDLE.PUT        diag1, "posY2"        ,  0.5
!BUNDLE.PUT        diag1, "posY2"        ,  0.95
BUNDLE.PUT        diag1, "cntDivX"      ,  8
BUNDLE.PUT        diag1, "cntDivY"      ,  9
BUNDLE.PUT        diag1, "border"       ,  0.09
BUNDLE.PUT        diag1, "borderCol"    ,  " 250 200 200 200 "
BUNDLE.PUT        diag1, "backGrCol"    ,  " 255 58  58  58  "
BUNDLE.PUT        diag1, "gridCol"      ,  " 255 50  100 50  "
BUNDLE.PUT        diag1, "lineCol"      ,  " 190 200 200  20  "
BUNDLE.PUT        diag1, "lineWidth"    ,  2
!BUNDLE.PUT        diag1, "numbersSize"  ,  16
BUNDLE.PUT        diag1, "numbersSize"  ,  30
BUNDLE.PUT        diag1, "nDigitXaxis"  ,  2
BUNDLE.PUT        diag1, "nDigitYaxis"  ,  4
BUNDLE.PUT        diag1, "updaterate"   ,  20
BUNDLE.PUT        diag1, "useMarker"    ,  0
BUNDLE.PUT        diag1, "markerSize"   ,  2
!BUNDLE.PUT        diag1, "useFillArea"  ,  1
BUNDLE.PUT        diag1, "useFillArea"  ,  0
BUNDLE.PUT        diag1, "alphaFillArea",  50
BUNDLE.PUT        diag1, "useTopAxis"   ,  0
BUNDLE.PUT        diag1, "useRightAxis" ,  0

CALL              PlotFrame(diag1)
FN.RETURN	%SetupPlot

!FN.DEF             plot (diag, xval [], yval [])
FN.DEF PlotFrame(diag)
 BUNDLE.GET        diag, "npoints"     ,  npoints
 BUNDLE.GET        diag, "xs"          ,  xs
 BUNDLE.GET        diag, "xe"          ,  xe
 BUNDLE.GET        diag, "ys"          ,  ys
 BUNDLE.GET        diag, "ye"          ,  ye
 BUNDLE.GET        diag, "posX1"       ,  posX1
 BUNDLE.GET        diag, "posY1"       ,  posY1
 BUNDLE.GET        diag, "posX2"       ,  posX2
 BUNDLE.GET        diag, "posY2"       ,  posY2
 BUNDLE.GET        diag, "cntDivX"     ,  cntDivX
 BUNDLE.GET        diag, "cntDivY"     ,  cntDivY
 BUNDLE.GET        diag, "border"      ,  border
 BUNDLE.GET        diag, "borderCol"   ,  borderCol$
 BUNDLE.GET        diag, "backGrCol"   ,  backGrCol$
 BUNDLE.GET        diag, "gridCol"     ,  gridCol$
 BUNDLE.GET        diag, "lineCol"     ,  lineCol$
 BUNDLE.GET        diag, "lineWidth"   ,  lineWidth
 BUNDLE.GET        diag, "numbersSize" ,  numbersSize
 BUNDLE.GET        diag, "nDigitXaxis" ,  nDigitXaxis
 BUNDLE.GET        diag, "nDigitYaxis" ,  nDigitYaxis
 BUNDLE.GET        diag, "updaterate"  ,  updaterate
 BUNDLE.GET        diag, "useMarker"   ,  useMarker
 BUNDLE.GET        diag, "markerSize"  ,  markerSize
 BUNDLE.GET        diag, "useFillArea" ,  useFillArea
 BUNDLE.GET        diag, "alphaFillArea", alphaFillArea
 BUNDLE.GET        diag, "useTopAxis"  ,  useTopAxis
 BUNDLE.GET        diag, "useRightAxis",  useRightAxis

 GR.SCREEN         curW, curH
 offsX          =  posX1 * curW
 offsY          =  posY1 * curH
 widX           =  ( posX2 - posX1 ) * curW
 widY           =  ( posY2 - posY1 ) * curH
 borderX        =  border * widX
 borderY        =  border * widY
 pixX           =  widX - (2 * borderX)
 pixY           =  widY - (2 * borderY)

 fmt$           =  "################"
 pScax          =  pixX / (xe-xs)
 pScay          =  pixY / (ye-ys)
 border         =  (border * ((pixX + pixy)/2))/2

 curCol$         = borderCol$
 curFill         = 1
 GOSUB setColor
 GR.RECT (nn, offsX, offsY, offsX + widx, offsY + widy)

 curCol$         = backGrCol$
 curFill         = 1
 GOSUB setColor
 GR.RECT (nn, offsX + borderx,offsY + bordery,offsX + pixX + borderx, offsY + pixY + bordery

 curCol$         = gridCol$
 GOSUB setColor

 GR.SET.STROKE     1
 GR.TEXT.SIZE      numbersSize
 GR.TEXT.ALIGN     2
 FOR i           = 0 TO cntDivX
  tmp            = offsX+ borderx + i*pixX/cntDivX
  fmtstr$        = "#############%."+right $(fmt$, nDigitXaxis)
  tmp$           = replace $(format$ ( fmtstr$, xs+i*(xe-xs)/cntDivX)," " ,"")
  GR.LINE          nn,  tmp, offsY+ bordery, tmp, offsY+ pixY+ bordery
  IF               useTopAxis THEN tmp1= -numbersSize*0.6 ELSE tmp1= pixY+ numbersSize*1.2
  GR.TEXT.DRAW     nn, tmp, offsY+ bordery+ tmp1 , tmp$
 NEXT

 IF                useRightAxis THEN GR.TEXT.ALIGN 1 ELSE GR.TEXT.ALIGN 3
 FOR i           = 0 TO cntDivY
  tmp            = offsY+ bordery + i*pixY/cntDivY
  fmtstr$        = "#############%."+right $(fmt$, nDigitXaxis)
  tmp$           = replace $(format$ ( fmtstr$, ye - i* (ye-ys) /cntDivY )," " ,"")
  GR.LINE          nn, offsX+ borderx, tmp, offsX+ pixX+ borderx , tmp
  IF useRightAxis THEN tmp1= pixX + 5 ELSE tmp1= -5
  GR.TEXT.DRAW     nn, offsX+ borderx + tmp1 , tmp , tmp$
 NEXT

 curCol$         = lineCol$
 curFill         = 1
 GOSUB             setColor
 GR.SET.STROKE     linewidth
 xo             =  offsX+ pixX+ borderx
 xu             =  offsX+       borderx
 yo             =  offsY+ pixY+ bordery
 yu             =  offsY+       bordery

 curCol$         = gridCol$
 GR.COLOR (255, VAL(WORD$(curCol$,2)), VAL(WORD$(curCol$,3)), VAL(WORD$(curCol$,4)), 0)
 GR.SET.STROKE     linewidth+2
 GR.RECT           nn, offsX+borderx,offsY+bordery,offsX+pixX+borderx, offsY+pixY+bordery
 GR.RENDER
 FN.RTN 1	%PlotFrame

 setColor:
 GR.COLOR  ~
 VAL(WORD$(curcol$,1)), VAL(WORD$(curCol$,2)), ~
 VAL(WORD$(curCol$,3)), VAL(WORD$(curCol$,4)), curFill
 RETURN

FN.END	%PlotFrame

!FN.DEF PlotDataValue(Xval, Yval)
FN.DEF PlotDataValue(device$, rmsg$)
   PRINT device$; ": "; rmsg$
FN.END	%PlotDataValue

RETURN	%userfunctions

!--------------------------------------------
openScreen:
refW       = 780
refH       = 1280
IF !flagOri  THEN SWAP refW, refH
centW      = refW/2
centH      = refH/2
!GR.OPEN      255,0,0,20,0,flagOri
GR.OPEN      255,0,0,20,1,flagOri		% Alpha, Red, Green, Blue, ShowStatusBar,Orientation
GR.SCREEN    curW, curH
scaW       = curW / refW
scaH       = curH / refH
!GR.SCALE     scaW , scaH
desLoopTime= 50
RETURN	%openScreen

!--------------------------------------------
onError:
END
!Last line.