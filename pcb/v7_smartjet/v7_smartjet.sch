EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Transistor_BJT:TIP122 Q6
U 1 1 6070E61D
P 10195 1395
F 0 "Q6" H 10402 1441 50  0000 L CNN
F 1 "TIP122" H 10402 1350 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 10395 1320 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/TIP120-D.PDF" H 10195 1395 50  0001 L CNN
	1    10195 1395
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q7
U 1 1 6070F233
P 10525 4705
F 0 "Q7" V 10760 4705 50  0000 C CNN
F 1 "BC547" V 10851 4705 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 10725 4630 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC550-D.pdf" H 10525 4705 50  0001 L CNN
	1    10525 4705
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 6070F7F5
P 7025 4340
F 0 "R2" H 7095 4386 50  0000 L CNN
F 1 "330R" H 7095 4295 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6955 4340 50  0001 C CNN
F 3 "~" H 7025 4340 50  0001 C CNN
	1    7025 4340
	1    0    0    -1  
$EndComp
$Comp
L esp32_devkit_v1_doit:ESP32_DevKit_V1_DOIT U2
U 1 1 6071030C
P 4465 2775
F 0 "U2" H 5015 4355 50  0000 C CNN
F 1 "ESP32_DevKit_V1_DOIT" H 5015 4270 50  0000 C CNN
F 2 "kicad_smartjet:esp32_devkit_v1" H 4015 4125 50  0001 C CNN
F 3 "https://aliexpress.com/item/32864722159.html" H 4015 4125 50  0001 C CNN
	1    4465 2775
	1    0    0    -1  
$EndComp
NoConn ~ 4365 4175
$Comp
L power:GND #PWR0101
U 1 1 60714C59
P 10295 1595
F 0 "#PWR0101" H 10295 1345 50  0001 C CNN
F 1 "GND" H 10300 1422 50  0000 C CNN
F 2 "" H 10295 1595 50  0001 C CNN
F 3 "" H 10295 1595 50  0001 C CNN
	1    10295 1595
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC557 Q5
U 1 1 60717FF2
P 9595 1600
F 0 "Q5" H 9786 1646 50  0000 L CNN
F 1 "BC557" H 9786 1555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 9795 1525 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC556BTA-D.pdf" H 9595 1600 50  0001 L CNN
	1    9595 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 6071C7F6
P 9695 1800
F 0 "#PWR0102" H 9695 1550 50  0001 C CNN
F 1 "GND" H 9700 1627 50  0000 C CNN
F 2 "" H 9695 1800 50  0001 C CNN
F 3 "" H 9695 1800 50  0001 C CNN
	1    9695 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9995 1395 9695 1395
Wire Wire Line
	9695 1395 9695 1400
$Comp
L Device:R R6
U 1 1 6071E269
P 9695 1105
F 0 "R6" H 9765 1151 50  0000 L CNN
F 1 "10k" H 9765 1060 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 9625 1105 50  0001 C CNN
F 3 "~" H 9695 1105 50  0001 C CNN
	1    9695 1105
	1    0    0    -1  
$EndComp
Wire Wire Line
	9695 1395 9695 1255
Connection ~ 9695 1395
$Comp
L Transistor_BJT:BC547 Q4
U 1 1 6072A905
P 9435 4770
F 0 "Q4" H 9626 4816 50  0000 L CNN
F 1 "BC547" H 9626 4725 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 9635 4695 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC550-D.pdf" H 9435 4770 50  0001 L CNN
	1    9435 4770
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q3
U 1 1 6072BEF5
P 8715 4770
F 0 "Q3" H 8906 4816 50  0000 L CNN
F 1 "BC547" H 8906 4725 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8915 4695 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC550-D.pdf" H 8715 4770 50  0001 L CNN
	1    8715 4770
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q2
U 1 1 6072C69E
P 7935 4745
F 0 "Q2" H 8126 4791 50  0000 L CNN
F 1 "BC547" H 8126 4700 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8135 4670 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC550-D.pdf" H 7935 4745 50  0001 L CNN
	1    7935 4745
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q1
U 1 1 6072CC79
P 6925 4750
F 0 "Q1" H 7116 4796 50  0000 L CNN
F 1 "BC547" H 7116 4705 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7125 4675 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC550-D.pdf" H 6925 4750 50  0001 L CNN
	1    6925 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 60733520
P 9535 4970
F 0 "#PWR0103" H 9535 4720 50  0001 C CNN
F 1 "GND" H 9540 4797 50  0000 C CNN
F 2 "" H 9535 4970 50  0001 C CNN
F 3 "" H 9535 4970 50  0001 C CNN
	1    9535 4970
	1    0    0    -1  
$EndComp
Wire Wire Line
	10270 4830 10270 4805
Wire Wire Line
	10270 4805 10325 4805
$Comp
L power:GND #PWR0104
U 1 1 60739D9F
P 8815 4970
F 0 "#PWR0104" H 8815 4720 50  0001 C CNN
F 1 "GND" H 8820 4797 50  0000 C CNN
F 2 "" H 8815 4970 50  0001 C CNN
F 3 "" H 8815 4970 50  0001 C CNN
	1    8815 4970
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 6073AFBE
P 8035 4945
F 0 "#PWR0105" H 8035 4695 50  0001 C CNN
F 1 "GND" H 8040 4772 50  0000 C CNN
F 2 "" H 8035 4945 50  0001 C CNN
F 3 "" H 8035 4945 50  0001 C CNN
	1    8035 4945
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 6073BCC4
P 7025 4950
F 0 "#PWR0106" H 7025 4700 50  0001 C CNN
F 1 "GND" H 7030 4777 50  0000 C CNN
F 2 "" H 7025 4950 50  0001 C CNN
F 3 "" H 7025 4950 50  0001 C CNN
	1    7025 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7025 4550 7025 4520
$Comp
L Device:R R3
U 1 1 6074177D
P 8035 4315
F 0 "R3" H 8105 4361 50  0000 L CNN
F 1 "330R" H 8105 4270 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7965 4315 50  0001 C CNN
F 3 "~" H 8035 4315 50  0001 C CNN
	1    8035 4315
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 6074374A
P 8815 4300
F 0 "R4" H 8885 4346 50  0000 L CNN
F 1 "330R" H 8885 4255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8745 4300 50  0001 C CNN
F 3 "~" H 8815 4300 50  0001 C CNN
	1    8815 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8815 4570 8815 4495
$Comp
L Device:R R5
U 1 1 60746928
P 9535 4295
F 0 "R5" H 9605 4341 50  0000 L CNN
F 1 "330R" H 9605 4250 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 9465 4295 50  0001 C CNN
F 3 "~" H 9535 4295 50  0001 C CNN
	1    9535 4295
	1    0    0    -1  
$EndComp
Wire Wire Line
	9535 4570 9535 4505
Text GLabel 6725 4750 0    50   Input ~ 0
ib_100
Text GLabel 7735 4745 3    50   Input ~ 0
ib_75
Text GLabel 8515 4770 3    50   Input ~ 0
ib_50
Text GLabel 9235 4770 3    50   Input ~ 0
ib_25
Text GLabel 7025 4190 1    50   Input ~ 0
100_leds-
Text GLabel 8035 4165 1    50   Input ~ 0
75_leds-
Text GLabel 8815 4150 1    50   Input ~ 0
50_leds-
Text GLabel 9535 4145 1    50   Input ~ 0
25_leds-
Text GLabel 10525 4505 1    50   Input ~ 0
Trigger_Leds_Battery
Text GLabel 10725 4805 2    50   Input ~ 0
Leds+
Text GLabel 10295 1195 1    50   Input ~ 0
pump-
Text GLabel 9395 1600 0    50   Input ~ 0
trigger_pump
Text GLabel 7095 4520 2    50   Input ~ 0
level_100
Wire Wire Line
	7025 4520 7095 4520
Connection ~ 7025 4520
Wire Wire Line
	7025 4520 7025 4490
Text GLabel 8100 4505 2    50   Input ~ 0
level_75
Wire Wire Line
	8100 4505 8035 4505
Wire Wire Line
	8035 4465 8035 4505
Connection ~ 8035 4505
Wire Wire Line
	8035 4505 8035 4545
Text GLabel 8930 4495 2    50   Input ~ 0
level_50
Wire Wire Line
	8930 4495 8815 4495
Connection ~ 8815 4495
Wire Wire Line
	8815 4495 8815 4450
Text GLabel 9630 4505 2    50   Input ~ 0
level_25
Wire Wire Line
	9630 4505 9535 4505
Connection ~ 9535 4505
Wire Wire Line
	9535 4505 9535 4445
Text GLabel 5065 2575 2    50   Input ~ 0
level_25
Text GLabel 5065 2675 2    50   Input ~ 0
level_50
Text GLabel 5065 2075 2    50   Input ~ 0
level_75
Text GLabel 5065 2775 2    50   Input ~ 0
level_100
Text GLabel 5065 1975 2    50   Input ~ 0
wake_up
Text GLabel 5065 2475 2    50   Input ~ 0
Trigger_Leds_Battery
Text GLabel 5065 1775 2    50   Input ~ 0
trigger_pump
NoConn ~ 5065 1675
NoConn ~ 5065 1875
NoConn ~ 5065 2175
NoConn ~ 5065 2275
NoConn ~ 5065 2375
NoConn ~ 5065 2975
NoConn ~ 5065 3075
NoConn ~ 5065 3275
NoConn ~ 5065 3375
NoConn ~ 5065 3475
NoConn ~ 5065 3575
NoConn ~ 5065 3675
NoConn ~ 5065 3775
NoConn ~ 5065 3875
$Comp
L power:GND #PWR0109
U 1 1 607DDC4D
P 4465 4175
F 0 "#PWR0109" H 4465 3925 50  0001 C CNN
F 1 "GND" H 4470 4002 50  0000 C CNN
F 2 "" H 4465 4175 50  0001 C CNN
F 3 "" H 4465 4175 50  0001 C CNN
	1    4465 4175
	1    0    0    -1  
$EndComp
NoConn ~ 3865 1575
NoConn ~ 3865 1775
NoConn ~ 3865 1875
Text GLabel 2995 3575 2    50   Input ~ 0
100_leds-
Text GLabel 2995 4175 2    50   Input ~ 0
ib_25
Text GLabel 2995 3975 2    50   Input ~ 0
25_leds-
Text GLabel 2995 4075 2    50   Input ~ 0
ib_50
Text GLabel 2995 3875 2    50   Input ~ 0
50_leds-
Text GLabel 2995 3775 2    50   Input ~ 0
ib_75
Text GLabel 2995 3675 2    50   Input ~ 0
75_leds-
Text GLabel 2995 4275 2    50   Input ~ 0
wake_up
Text GLabel 5365 3175 2    50   Input ~ 0
status_led+
$Comp
L Device:R R1
U 1 1 607AD099
P 5215 3175
F 0 "R1" V 5008 3175 50  0000 C CNN
F 1 "330R" V 5099 3175 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5145 3175 50  0001 C CNN
F 3 "~" H 5215 3175 50  0001 C CNN
	1    5215 3175
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR0118
U 1 1 6072FC4B
P 10270 4830
F 0 "#PWR0118" H 10270 4680 50  0001 C CNN
F 1 "VCC" H 10285 5003 50  0000 C CNN
F 2 "" H 10270 4830 50  0001 C CNN
F 3 "" H 10270 4830 50  0001 C CNN
	1    10270 4830
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 608A92E0
P 1325 1300
F 0 "#FLG0101" H 1325 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 1325 1473 50  0000 C CNN
F 2 "" H 1325 1300 50  0001 C CNN
F 3 "~" H 1325 1300 50  0001 C CNN
	1    1325 1300
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 608A97FA
P 1850 1290
F 0 "#FLG0102" H 1850 1365 50  0001 C CNN
F 1 "PWR_FLAG" H 1850 1463 50  0000 C CNN
F 2 "" H 1850 1290 50  0001 C CNN
F 3 "~" H 1850 1290 50  0001 C CNN
	1    1850 1290
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0119
U 1 1 608AA508
P 1325 1300
F 0 "#PWR0119" H 1325 1150 50  0001 C CNN
F 1 "VCC" H 1340 1473 50  0000 C CNN
F 2 "" H 1325 1300 50  0001 C CNN
F 3 "" H 1325 1300 50  0001 C CNN
	1    1325 1300
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 608AB228
P 1850 1290
F 0 "#PWR0120" H 1850 1040 50  0001 C CNN
F 1 "GND" H 1855 1117 50  0000 C CNN
F 2 "" H 1850 1290 50  0001 C CNN
F 3 "" H 1850 1290 50  0001 C CNN
	1    1850 1290
	1    0    0    -1  
$EndComp
Text GLabel 2995 3475 2    50   Input ~ 0
ib_100
Text GLabel 1615 2255 2    50   Input ~ 0
pump-
$Comp
L power:GND #PWR0111
U 1 1 607E8084
P 1615 2355
F 0 "#PWR0111" H 1615 2105 50  0001 C CNN
F 1 "GND" H 1620 2182 50  0000 C CNN
F 2 "" H 1615 2355 50  0001 C CNN
F 3 "" H 1615 2355 50  0001 C CNN
	1    1615 2355
	1    0    0    -1  
$EndComp
Text GLabel 2995 3375 2    50   Input ~ 0
status_led+
Text GLabel 2995 4375 2    50   Input ~ 0
Leds+
$Comp
L power:VCC #PWR0110
U 1 1 607E5FD1
P 1615 2155
F 0 "#PWR0110" H 1615 2005 50  0001 C CNN
F 1 "VCC" V 1630 2283 50  0000 L CNN
F 2 "" H 1615 2155 50  0001 C CNN
F 3 "" H 1615 2155 50  0001 C CNN
	1    1615 2155
	0    1    1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x03 J2
U 1 1 608270DF
P 1415 2255
F 0 "J2" H 1333 1930 50  0000 C CNN
F 1 "Screw_Terminal_01x03" H 1333 2021 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 1415 2255 50  0001 C CNN
F 3 "~" H 1415 2255 50  0001 C CNN
	1    1415 2255
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR0107
U 1 1 6086984E
P 9695 955
F 0 "#PWR0107" H 9695 805 50  0001 C CNN
F 1 "VCC" V 9710 1083 50  0000 L CNN
F 2 "" H 9695 955 50  0001 C CNN
F 3 "" H 9695 955 50  0001 C CNN
	1    9695 955 
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0112
U 1 1 6086F5AA
P 4365 1375
F 0 "#PWR0112" H 4365 1225 50  0001 C CNN
F 1 "VCC" V 4380 1503 50  0000 L CNN
F 2 "" H 4365 1375 50  0001 C CNN
F 3 "" H 4365 1375 50  0001 C CNN
	1    4365 1375
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x11_Female J1
U 1 1 608B0322
P 2795 3875
F 0 "J1" H 2823 3901 50  0000 L CNN
F 1 "Conn_01x11_Female" H 2823 3810 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x11_P2.54mm_Horizontal" H 2795 3875 50  0001 C CNN
F 3 "~" H 2795 3875 50  0001 C CNN
	1    2795 3875
	-1   0    0    1   
$EndComp
NoConn ~ 4465 1375
NoConn ~ 5065 2875
NoConn ~ 7730 2510
$EndSCHEMATC
