module Top (

	input clock,
	input reset,
	
	input requestHW,
	input [31:0]address,
	input [31:0]dataFromNIOS,
	input [1:0] direcaoFromNIOS

);

parameter tamanhoDados = 8 * $clog2(TamanhoMalha) + 3;
parameter TamanhoMalha = 15;

//NIOS
/*
wire [tamanhoDados - 1 : 0] dataFromNIOS;
wire [1:0] direcaoFromNIOS;
wire valueFIFO;
wire emptyFIFO;
wire readFIFO;
wire requestHW;
*/
//

vizinhoTop #(
	.TamanhoMalha(TamanhoMalha)
) vizinhos 
(

	.clock(clock),
	.reset(reset),
	.request(requestHW),
	
	.address(address),
	.value(dataFromNIOS),
	.direcao(direcaoFromNIOS),
	
	.addressWrite(addressFromHW),
	.dataToMalha(valueFromHW),
	.dataFromMalha(outMalha),
	.readMalha(readFromHW),
	.writeMalha(writeFromHW),
	
	.writeFIFO(writeFIFO),
	.dataFIFO(valueFIFO) 

); 


Malha #(.TamanhoMalha(TamanhoMalha*TamanhoMalha), .tamanhoDados(tamanhoDados))
Malha (

    .clock(clock),
    .reset(reset),

    .value(valueToMalha),
    .read(readToMalha), 
    .write(writeToMalha),
    .address(addressToMalha),

    .outData(outMalha)
    
);

decisorMalha decisor(

	.addressFromNIOS(addresFromNIOS),
	.readFromNIOS(readFromNIOS),
	.writeFromNIOS(writeFromNIOS),
	.valueFromNIOS(valueFromNIOS),

	.addressFromHW(addressFromHW),
	.readFromHW(readFromHW),
	.writeFromHW(writeFromHW),
	.valueFromHW(valueFromHW),
	
	//Dados para a Malha
	.address(addresToMalha),
	.read(readToMalha),
	.write(writeToMalha),
	.value(valueToMalha)

);

FIFO#(.DEPTH(8), .DWIDTH(16)) 
filaHeap (

	.rstn(!reset),               // Active low reset
	.clk(clock),                // Clock
	
	.din(valueFIFO),
	.dout(dataFIFO),
	
	.wr_en(writeFIFO), 				// Write enable
	.rd_en(readFIFO), 				// Read enable
	
	.empty(emptyFIFO) 				// FIFO is empty when high
	
);

endmodule