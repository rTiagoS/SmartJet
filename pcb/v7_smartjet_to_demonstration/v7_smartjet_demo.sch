EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "SmartJet PCB For Demonstration"
Date ""
Rev "v1"
Comp "IACINOVATION"
Comment1 "Implemented by: Tiago Sá."
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L esp32_devkit_v1_doit:ESP32_DevKit_V1_DOIT U1
U 1 1 608C8BC2
P 4670 2975
F 0 "U1" H 4670 4750 50  0000 C CNN
F 1 "ESP32_DevKit_V1_DOIT" H 4640 4665 50  0000 C CNN
F 2 "kicad_smartjet:esp32_devkit_v1" H 4220 4325 50  0001 C CNN
F 3 "https://aliexpress.com/item/32864722159.html" H 4220 4325 50  0001 C CNN
	1    4670 2975
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0101
U 1 1 608C9CAD
P 4570 1575
F 0 "#PWR0101" H 4570 1425 50  0001 C CNN
F 1 "VCC" H 4585 1748 50  0000 C CNN
F 2 "" H 4570 1575 50  0001 C CNN
F 3 "" H 4570 1575 50  0001 C CNN
	1    4570 1575
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC557 Q1
U 1 1 608CCD9F
P 8380 1675
F 0 "Q1" H 8571 1721 50  0000 L CNN
F 1 "BC557" H 8571 1630 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8580 1600 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC556BTA-D.pdf" H 8380 1675 50  0001 L CNN
	1    8380 1675
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:TIP122 Q2
U 1 1 608CD6EC
P 9015 1445
F 0 "Q2" H 9222 1491 50  0000 L CNN
F 1 "TIP122" H 9222 1400 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9215 1370 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/TIP120-D.PDF" H 9015 1445 50  0001 L CNN
	1    9015 1445
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 608CED31
P 8480 1245
F 0 "R1" H 8550 1291 50  0000 L CNN
F 1 "10k" H 8550 1200 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8410 1245 50  0001 C CNN
F 3 "~" H 8480 1245 50  0001 C CNN
	1    8480 1245
	1    0    0    -1  
$EndComp
Wire Wire Line
	8480 1475 8480 1445
$Comp
L power:VCC #PWR0102
U 1 1 608D0093
P 8480 1095
F 0 "#PWR0102" H 8480 945 50  0001 C CNN
F 1 "VCC" H 8495 1268 50  0000 C CNN
F 2 "" H 8480 1095 50  0001 C CNN
F 3 "" H 8480 1095 50  0001 C CNN
	1    8480 1095
	1    0    0    -1  
$EndComp
Wire Wire Line
	8815 1445 8480 1445
Connection ~ 8480 1445
Wire Wire Line
	8480 1445 8480 1395
$Comp
L power:GND #PWR0103
U 1 1 608D2C4B
P 9115 1645
F 0 "#PWR0103" H 9115 1395 50  0001 C CNN
F 1 "GND" H 9120 1472 50  0000 C CNN
F 2 "" H 9115 1645 50  0001 C CNN
F 3 "" H 9115 1645 50  0001 C CNN
	1    9115 1645
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 608D39B2
P 8480 1875
F 0 "#PWR0104" H 8480 1625 50  0001 C CNN
F 1 "GND" H 8485 1702 50  0000 C CNN
F 2 "" H 8480 1875 50  0001 C CNN
F 3 "" H 8480 1875 50  0001 C CNN
	1    8480 1875
	1    0    0    -1  
$EndComp
Text GLabel 8180 1675 0    50   Input ~ 0
trigger_pump
Text GLabel 9115 1245 1    50   Input ~ 0
pump-
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 608D6620
P 1635 1045
F 0 "#FLG0101" H 1635 1120 50  0001 C CNN
F 1 "PWR_FLAG" H 1635 1218 50  0000 C CNN
F 2 "" H 1635 1045 50  0001 C CNN
F 3 "~" H 1635 1045 50  0001 C CNN
	1    1635 1045
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 608D698D
P 2165 1040
F 0 "#FLG0102" H 2165 1115 50  0001 C CNN
F 1 "PWR_FLAG" H 2165 1213 50  0000 C CNN
F 2 "" H 2165 1040 50  0001 C CNN
F 3 "~" H 2165 1040 50  0001 C CNN
	1    2165 1040
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0105
U 1 1 608D6F21
P 1635 1045
F 0 "#PWR0105" H 1635 895 50  0001 C CNN
F 1 "VCC" H 1650 1218 50  0000 C CNN
F 2 "" H 1635 1045 50  0001 C CNN
F 3 "" H 1635 1045 50  0001 C CNN
	1    1635 1045
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 608D8308
P 2165 1040
F 0 "#PWR0106" H 2165 790 50  0001 C CNN
F 1 "GND" H 2170 867 50  0000 C CNN
F 2 "" H 2165 1040 50  0001 C CNN
F 3 "" H 2165 1040 50  0001 C CNN
	1    2165 1040
	1    0    0    -1  
$EndComp
Text GLabel 5270 1975 2    50   Input ~ 0
trigger_pump
$Comp
L Connector:Screw_Terminal_01x04 J1
U 1 1 608DBD0A
P 1680 2430
F 0 "J1" H 1598 2005 50  0000 C CNN
F 1 "Screw_Terminal_01x04" H 1598 2096 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-4_P5.08mm" H 1680 2430 50  0001 C CNN
F 3 "~" H 1680 2430 50  0001 C CNN
	1    1680 2430
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR0107
U 1 1 608DDB02
P 1880 2530
F 0 "#PWR0107" H 1880 2380 50  0001 C CNN
F 1 "VCC" V 1895 2658 50  0000 L CNN
F 2 "" H 1880 2530 50  0001 C CNN
F 3 "" H 1880 2530 50  0001 C CNN
	1    1880 2530
	-1   0    0    1   
$EndComp
NoConn ~ 4670 1575
Text GLabel 1880 2330 2    50   Input ~ 0
pump-
$Comp
L power:GND #PWR0108
U 1 1 608E0D37
P 4670 4375
F 0 "#PWR0108" H 4670 4125 50  0001 C CNN
F 1 "GND" H 4675 4202 50  0000 C CNN
F 2 "" H 4670 4375 50  0001 C CNN
F 3 "" H 4670 4375 50  0001 C CNN
	1    4670 4375
	1    0    0    -1  
$EndComp
Text GLabel 1880 2430 2    50   Input ~ 0
out_wake_up_pin
$Comp
L power:GND #PWR0109
U 1 1 608E4225
P 1880 2230
F 0 "#PWR0109" H 1880 1980 50  0001 C CNN
F 1 "GND" H 1885 2057 50  0000 C CNN
F 2 "" H 1880 2230 50  0001 C CNN
F 3 "" H 1880 2230 50  0001 C CNN
	1    1880 2230
	0    -1   -1   0   
$EndComp
Text GLabel 5270 2175 2    50   Input ~ 0
out_wake_up_pin
NoConn ~ 5270 1875
NoConn ~ 5270 2075
NoConn ~ 5270 2275
NoConn ~ 5270 2375
NoConn ~ 5270 2475
NoConn ~ 5270 2575
NoConn ~ 5270 2675
NoConn ~ 5270 2775
NoConn ~ 5270 2875
NoConn ~ 5270 2975
NoConn ~ 5270 3075
NoConn ~ 5270 3175
NoConn ~ 5270 3275
NoConn ~ 5270 3375
NoConn ~ 5270 3475
NoConn ~ 5270 3575
NoConn ~ 5270 3675
NoConn ~ 5270 3775
NoConn ~ 5270 3875
NoConn ~ 5270 3975
NoConn ~ 5270 4075
NoConn ~ 4570 4375
NoConn ~ 4070 1775
NoConn ~ 4070 1975
NoConn ~ 4070 2075
$EndSCHEMATC