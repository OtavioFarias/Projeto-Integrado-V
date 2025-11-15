module Top 
#(
parameter int TamanhoMalha = 15, 
parameter int tamanhoDados = 6 * $clog2(TamanhoMalha) + 5,
parameter addressTam = 2 * $clog2(TamanhoMalha)
)
(

	input clock,
	input reset,
	
	input requestFromNIOS,
	input [addressTam - 1 : 0] addressFromNIOS,
	input [tamanhoDados - 1:0] dataFromNIOS,
	
	input readFromNIOS,
	input writeFromNIOS,
	input readFIFO,
	
	output ready

);

wire [addressTam - 1 : 0] addressWrite, custoAcumulado, addressToHW;
wire [tamanhoDados - 1:0] dataToMalha, dataToHW;
wire [addressTam : 0] dataFIFO, valueFIFO;

wire	 [tamanhoDados - 1:0] valueFromNIOS;

wire	 [addressTam - 1 : 0] addressFromHW;
wire	 readFromHW;
wire	 writeFromHW;
wire requestToHW;
wire	 [tamanhoDados - 1:0] valueFromHW;
	
wire	 [addressTam - 1 : 0] addressToMalha;
wire	 readToMalha;
wire	 writeToMalha;
wire [1:0] direcaoToHW;
wire	 [tamanhoDados - 1:0] valueToMalha;
	
wire [tamanhoDados - 1:0] outMalha;

wire readyHW;

wire writeFIFO;
wire emptyFIFO;

wire lastUseFromControler, lastUseFromHW;

//NIOS
/*
wire [tamanhoDados - 1 : 0] dataFromNIOS;
wire valueFIFO;
wire emptyFIFO;
wire readFIFO;
wire requestHW;
*/

assign ready = readyHW;

//

controler 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam)
)Controlador
(

	.clock(clock),
	.reset(reset),

	.requestFromNIOS(requestFromNIOS),
	.ready(readyHW),

	.addressFromNIOS(addressFromNIOS),
	.dataFromNIOS(dataFromNIOS),
	.write(writeFromHW),

	.requestToHW(requestToHW),
	.addressToHW(addressToHW),
	.dataToHW(dataToHW),
	.direcaoToHW(direcaoToHW),
	
	.lastUse(lastUseFromControler),
	.lastUseInput(lastUseFromHW)
	
);


vizinhoTop 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
vizinhos 
(

	.clock(clock),
	.reset(reset),
	.request(requestToHW),
	
	.address(addressToHW),
	.value(dataToHW),
	.direcao(direcaoToHW),
	
	.addressWrite(addressFromHW),
	.dataToMalha(valueFromHW),
	.dataFromMalha(outMalha),
	.readMalha(readFromHW),
	.writeMalha(writeFromHW),
	
	.writeFIFO(writeFIFO),
	.dataFIFO(valueFIFO),
	
	.lastUse(lastUseFromControler),
	.lastUseOutput(lastUseFromHW)

); 


Malha #(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .tamanhoAddr(addressTam))
Malha (

    .clock(clock),
    .reset(reset),

    .value(valueToMalha),
    .read(readToMalha), 
    .write(writeToMalha),
    .address(addressToMalha),

    .outData(outMalha)
    
);

decisorMalha 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
decisor(

	.addressFromNIOS(addressFromNIOS),
	.readFromNIOS(readFromNIOS),
	.writeFromNIOS(writeFromNIOS),
	.valueFromNIOS(dataFromNIOS),

	.addressFromHW(addressFromHW),
	.readFromHW(readFromHW),
	.writeFromHW(writeFromHW),
	.valueFromHW(valueFromHW),
	
	//Dados para a Malha
	.address(addressToMalha),
	.read(readToMalha),
	.write(writeToMalha),
	.value(valueToMalha)

);

FIFO#(.DEPTH(8), .DWIDTH(addressTam + 1)) 
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