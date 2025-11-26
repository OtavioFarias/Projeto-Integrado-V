module decisorMalha 
#(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha)
)(

	input [addressTam - 1:0] addressFromNIOS,
	input readFromNIOS,
	input writeFromNIOS,
	input [tamanhoDados - 1:0] valueFromNIOS,

	input [addressTam - 1:0] addressFromHW,
	input readFromHW,
	input writeFromHW,
	input [tamanhoDados - 1:0] valueFromHW,
	
	//Dados para a Malha
	output [addressTam - 1:0] address,
	output read,
	output write,
	output [tamanhoDados - 1:0] value
	
);

wire requestFromHW;
assign requestFromHW = readFromHW || writeFromHW;

assign address = (requestFromHW) ? addressFromHW: addressFromNIOS;
assign read = (requestFromHW) ? readFromHW : readFromNIOS;
assign write = (requestFromHW) ? writeFromHW : writeFromNIOS;
assign value = (requestFromHW) ? valueFromHW : valueFromNIOS;

endmodule
