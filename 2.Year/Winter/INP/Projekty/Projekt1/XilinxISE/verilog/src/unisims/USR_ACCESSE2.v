// $Header: /devl/xcs/repo/env/Databases/CAEInterfaces/verunilibs/data/fuji/USR_ACCESSE2.v,v 1.1 2010/05/27 18:52:38 yanx Exp $
///////////////////////////////////////////////////////
//  Copyright (c) 2009 Xilinx Inc.
//  All Right Reserved.
///////////////////////////////////////////////////////
//
//   ____   ___
//  /   /\/   / 
// /___/  \  /     Vendor      : Xilinx 
// \  \    \/      Version : 10.1
//  \  \           Description : 
//  /  /                      
// /__/   /\       Filename    : USR_ACCESSE2.v
// \  \  /  \ 
//  \__\/\__ \                    
//                                 
//  Generated by :	/home/chen/xfoundry/HEAD/env/Databases/CAEInterfaces/LibraryWriters/bin/ltw.pl
//  Revision:		1.0
///////////////////////////////////////////////////////

`timescale 1 ps / 1 ps 

module USR_ACCESSE2 (
  CFGCLK,
  DATA,
  DATAVALID
);


  output CFGCLK;
  output DATAVALID;
  output [31:0] DATA;



  tri0 GSR = glbl.GSR;

  wire CFGCLK_OUT;
  wire DATAVALID_OUT;
  wire [31:0] DATA_OUT;



  initial begin
  end

  buf B_CFGCLK (CFGCLK, CFGCLK_OUT);
  buf B_DATA0 (DATA[0], DATA_OUT[0]);
  buf B_DATA1 (DATA[1], DATA_OUT[1]);
  buf B_DATA10 (DATA[10], DATA_OUT[10]);
  buf B_DATA11 (DATA[11], DATA_OUT[11]);
  buf B_DATA12 (DATA[12], DATA_OUT[12]);
  buf B_DATA13 (DATA[13], DATA_OUT[13]);
  buf B_DATA14 (DATA[14], DATA_OUT[14]);
  buf B_DATA15 (DATA[15], DATA_OUT[15]);
  buf B_DATA16 (DATA[16], DATA_OUT[16]);
  buf B_DATA17 (DATA[17], DATA_OUT[17]);
  buf B_DATA18 (DATA[18], DATA_OUT[18]);
  buf B_DATA19 (DATA[19], DATA_OUT[19]);
  buf B_DATA2 (DATA[2], DATA_OUT[2]);
  buf B_DATA20 (DATA[20], DATA_OUT[20]);
  buf B_DATA21 (DATA[21], DATA_OUT[21]);
  buf B_DATA22 (DATA[22], DATA_OUT[22]);
  buf B_DATA23 (DATA[23], DATA_OUT[23]);
  buf B_DATA24 (DATA[24], DATA_OUT[24]);
  buf B_DATA25 (DATA[25], DATA_OUT[25]);
  buf B_DATA26 (DATA[26], DATA_OUT[26]);
  buf B_DATA27 (DATA[27], DATA_OUT[27]);
  buf B_DATA28 (DATA[28], DATA_OUT[28]);
  buf B_DATA29 (DATA[29], DATA_OUT[29]);
  buf B_DATA3 (DATA[3], DATA_OUT[3]);
  buf B_DATA30 (DATA[30], DATA_OUT[30]);
  buf B_DATA31 (DATA[31], DATA_OUT[31]);
  buf B_DATA4 (DATA[4], DATA_OUT[4]);
  buf B_DATA5 (DATA[5], DATA_OUT[5]);
  buf B_DATA6 (DATA[6], DATA_OUT[6]);
  buf B_DATA7 (DATA[7], DATA_OUT[7]);
  buf B_DATA8 (DATA[8], DATA_OUT[8]);
  buf B_DATA9 (DATA[9], DATA_OUT[9]);
  buf B_DATAVALID (DATAVALID, DATAVALID_OUT);


  specify

    specparam PATHPULSE$ = 0;
  endspecify
endmodule