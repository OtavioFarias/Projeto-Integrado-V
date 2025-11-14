parameter addressTam = 2 * $clog2(TamanhoMalha);

module vizinhoTop 
#(
parameter int TamanhoMalha = 15, 
parameter int tamanhoDados = 6 * $clog2(TamanhoMalha) + 5
)
(

	input clock,
	input reset,
	
	input [addressTam - 1 : 0] address,
	input [tamanhoDados - 1:0] value,
	input [1:0] direcao, //00) esquerda 01) frente 2) tras 3) direita 
	
	output reg [addressTam - 1:0] addressWrite,
	input [tamanhoDados - 1:0] dataFromMalha

); 

parameter areaMalha = TamanhoMalha*TamanhoMalha;

wire [TamanhoMalha - 1 : 0] x, y;
wire [addressTam - 1 : 0] custoAcumulado;
wire [addressTam - 1 : 0] custoEstimado;
wire [1 : 0] pai;
wire aberto;
wire valorCelula; // diz se está ocupado, livre, fronteira ou desconhecido

assign { 
	x, y,
	valorCelula,
	pai,
	custoAcumulado,
	custoEstimado,
	aberto 
	
	} = value;

logic [addressTam - 1 : 0] muxDirecao;

always_comb begin

	case(direcao) //00) esquerda 01) frente 2) tras 3) direita 
		
		2'b00: begin
		
			muxDirecao = -1;
		
		end
		
		2'b01: begin
		
			muxDirecao = TamanhoMalha;
		
		end
		
		2'b10: begin
		
			muxDirecao = -TamanhoMalha;
		
		end
		
		2'b11: begin
		
			muxDirecao = 1;
		
		end
		
	endcase
	
end

wire [addressTam - 1:0] proximoVizinho;
assign proximoVizinho = muxDirecao + address;

wire [addressTam - 1:0] addressProximoVizinho;

S0 s0 (

	.clock(clock),
	.reset(reset),
	
	.proximoVizinho(proximoVizinho),
	.proximoVizinhoOut(addressProximoVizinho),
	
	.custoAcumulado(custoAcumulado),
	.custoAcumuladoOut(custoAcumuladoS1),
	
	.direcao(direcao),
	.direcaoOut(direcaoS1),
		
	.x(x),
	.xOut(xS1),
	
	.y(y),
	.yOut(yOriginalS1)
	
);

assign addressWrite = addressProximoVizinho; // no futuro um mux que vai poder escrever dados na malha

wire [addressTam - 1 : 0] addressS2;
wire [addressTam - 1 : 0] custoAcumuladoS2;

S1 s1 (

	.clock(clock),
	.reset(reset),
	
	.address(addressProximoVizinho),
	.addressOut(addressS2),
	
	.valueFromMalha(dataFromMalha),
	.valueFromMalhaOut(dataFromMalhaS2),
	
	.custoAcumulado(custoAcumuladoS1),
	.custoAcumuladoOut(custoAcumuladoS2),
	
	.direcao(direcaoS1),
	.direcaoOut(direcaoS2),
		
	.x(xOriginalS1),
	.xOut(xOriginalS2),
	
	.y(yOriginalS1),
	.yOut(yOriginalS2)
		
);

wire [TamanhoMalha - 1 : 0] xS2, yS2;
wire [addressTam - 1 : 0] custoEstimadoS2;
wire [1 : 0] paiS2;
wire abertoS2;

assign {
		 xS2, yS2,
         valorCelulaS2,
         paiS2,
         custoAcumuladoS2,
         custoEstimadoS2,
         abertoS2 } = dataFromMalhaS2;


wire novoCustoAcumuladoS2;
assign novoCustoAcumuladoS2 = custoAcumuladoS2 + 1;

wire celulaValorValido = (valorCelulaS2 == 0) || (valorCelulaS2 == 3);
wire addressValid = (addressS2 < areaMalha) && (addressS2 > 0);
wire valorValido = celulaValorValido && addressValid;

wire candidato = !abertoS2 || novoCustoAcumuladoS2 < custoAcumuladoS2;
//somador

S2 s2 (

	.clock(clock),
	.reset(reset),
	.flush(!(valorValido && candidato)),
	
	.custoAcumulado(novoCustoAcumuladoS2),
	.custoAcumuladoOut(custoAcumuladoS3), //vai esperar até o último estágio
	
	.address(addressS2),
	.addressOut(addressS3),
	
	.direcao(direcaoS2),
	.direcaoOut(direcaoS3),
	
	.x(xOriginalS2),
	.xOut(xOriginalS3),
	
	.y(yOriginalS2),
	.yOut(yOriginalS3),
	
	.xOriginal(xS2),
	.xOriginalOut(xS3),
	
	.yOriginal(yS2),
	.yOriginalOut(yS3)

);

wire [TamanhoMalha - 1 : 0] dx;
assign dx = xS3 - xOriginalS3;

wire [TamanhoMalha - 1 : 0] dy;
assign dy = yS3 - yOriginalS3;

wire [TamanhoMalha - 1 : 0] dx_;
assign dx_ = (dx[TamanhoMalha - 1]) ? !dx : dx;

wire [TamanhoMalha - 1 : 0] dy_;
assign dy_ = (dy[TamanhoMalha - 1]) ? !dy : dy;

wire [TamanhoMalha - 1 : 0] distanciaTotal = dx_ + dy_;

S3 s3 (

	.clock(clock),
	.reset(reset),
	
	.custoAcumulado(novoCustoAcumuladoS3),
	.custoAcumuladoOut(custoAcumuladoS4), 
	
	.custoEstimado(custoEstimado),
	.custoEstimadoOut(custoEstimadoS4), 
	
	.address(addressS3),
	.addressOut(addressS4),
	
	.direcao(direcaoS3),
	.direcaoOut(direcaoS4),
	
	.distanciaTotal(distanciaTotal),
	.distanciaTotalOut(distanciaTotalS4)
	
);

//envio para fila -> heap

endmodule