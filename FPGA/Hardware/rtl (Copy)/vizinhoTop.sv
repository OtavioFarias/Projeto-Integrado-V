module vizinhoTop 
#(
parameter int TamanhoMalha = 15, 
parameter int tamanhoDados = 6 * $clog2(TamanhoMalha) + 5,
parameter addressTam = 2 * $clog2(TamanhoMalha),
parameter linear = $clog2(TamanhoMalha)
)
(

	input clock,
	input reset,
	
	input request,
	input lastUse,
	output lastUseOutput,
	
	input [addressTam - 1 : 0] address,
	input [tamanhoDados - 1:0] value,
	input [1:0] direcao, //00) esquerda 01) frente 2) tras 3) direita 
	
	output writeMalha, readMalha,
	output [addressTam - 1:0] addressWrite,
	output [tamanhoDados - 1:0] dataToMalha,
	input [tamanhoDados - 1:0] dataFromMalha,
	
	output writeFIFO,
	output [addressTam : 0] dataFIFO

); 

parameter areaMalha = TamanhoMalha*TamanhoMalha;

wire [tamanhoDados - 1:0] dataFromMalhaS2;

wire [linear - 1 : 0] x, y, xS4, yS4, xS5, yS5, xOriginalS1	, yOriginalS1, xOriginalS2, yOriginalS2, xOriginalS3, yOriginalS3, xS3, yS3, xS2, yS2;

wire [addressTam - 1 : 0] custoAcumulado, custoAcumuladoS1, custoAcumuladoS3, custoAcumuladoS4, custoAcumuladoS5, custoEstimado, custoEstimadoS4, custoEstimadoS5, custoAcumuladoS2, novoCustoAcumuladoS2, custoVizinhoAcumuladoS2;

wire [addressTam - 1 : 0] addressS4, addressS5, addressS3, addressProximoVizinho, proximoVizinho, addressS2;

wire [1 : 0] pai, paiS2;

wire aberto, abertoS3, abertoS2, abertoS4, abertoS5;

wire [1:0] direcaoS1, direcaoS2, direcaoS3, direcaoS4, direcaoS5;

wire [1:0] valorCelula, valorS2, valorS1, valorS3, valorS4, valorS5; // diz se está ocupado, livre, fronteira ou desconhecido

wire [addressTam - 1 : 0] custoEstimadoS2, novoCustoEstimado;

wire celulaValorValido, addressValid, valorValido, candidato;

wire [TamanhoMalha - 1 : 0] dx, dy, dxS4, dyS4, dx_, dy_;

wire [addressTam - 1 : 0] distanciaTotalS5, distanciaTotal;

wire requestS1, requestS2, candidatoS2, candidatoS3, candidatoS4, candidatoS5;

wire lastUseS1, lastUseS2, lastUseS3, lastUseS4, lastUseS5;

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

assign proximoVizinho = muxDirecao + address;

S0 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
s0 (

	.clock(clock),
	.reset(reset),
	
	.proximoVizinho(proximoVizinho),
	.proximoVizinhoOut(addressProximoVizinho),
	
	.custoAcumulado(custoAcumulado),
	.custoAcumuladoOut(custoAcumuladoS1),
	
	.direcao(direcao),
	.direcaoOut(direcaoS1),
		
	.x(x),
	.xOut(xOriginalS1),
	
	.y(y),
	.yOut(yOriginalS1),
	
	.request(request),
	.requestOut(requestS1),
	
	.lastUse(lastUse),
	.lastUseOut(lastUseS1)
	
);

assign readMalha = requestS1;
assign addressWrite = (writeMalha) ? addressS5 : addressProximoVizinho;

S1 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
s1 (

	.clock(clock),
	.reset(reset),
	
	.address(addressProximoVizinho),
	.addressOut(addressS2),
	
	.custoAcumulado(custoAcumuladoS1),
	.custoAcumuladoOut(custoAcumuladoS2),
	
	.direcao(direcaoS1),
	.direcaoOut(direcaoS2),
		
	.x(xOriginalS1),
	.xOut(xOriginalS2),
	
	.y(yOriginalS1),
	.yOut(yOriginalS2),
	
	.request(requestS1),
	.requestOut(requestS2),
		
	.lastUse(lastUseS1),
	.lastUseOut(lastUseS2)
		
);

assign {
		 xS2, yS2,
         valorS2,
         paiS2,
         custoVizinhoAcumuladoS2,
         custoEstimadoS2,
         abertoS2 } = dataFromMalha;

assign novoCustoAcumuladoS2 = custoAcumuladoS2 + 1;

assign celulaValorValido = (valorS2 == 1 || valorS2 == 3);
assign addressValid = (addressS2 < areaMalha) && (addressS2 > 0);
assign valorValido = celulaValorValido && addressValid;
assign candidato = !abertoS2 || novoCustoAcumuladoS2 < custoVizinhoAcumuladoS2;

assign candidatoS2 = candidato && requestS2 && valorValido;

S2 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
s2 (

	.clock(clock),
	.reset(reset),
	
	.custoAcumulado(novoCustoAcumuladoS2),
	.custoAcumuladoOut(custoAcumuladoS3), //vai esperar até o último estágio
	
	.address(addressS2),
	.addressOut(addressS3),
	
	.direcao(direcaoS2),
	.direcaoOut(direcaoS3),
	
	.aberto(abertoS2),
	.abertoOut(abertoS3),
	
	.valor(valorS2),
	.valorOut(valorS3),
	
	.x(xS2),
	.xOut(xS3),
	
	.y(yS2),
	.yOut(yS3),
	
	.xOriginal(xOriginalS2),
	.xOriginalOut(xOriginalS3),
	
	.yOriginal(yOriginalS2),
	.yOriginalOut(yOriginalS3),
	
	.candidato(candidatoS2),
	.candidatoOut(candidatoS3),
	
	.lastUse(lastUseS2),
	.lastUseOut(lastUseS3)

);

assign dx = xS3 - xOriginalS3;
assign dy = yS3 - yOriginalS3;


S3 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
s3 (

	.clock(clock),
	.reset(reset),
	
	.custoAcumulado(custoAcumuladoS3),
	.custoAcumuladoOut(custoAcumuladoS4), 
	
	.custoEstimado(custoEstimado),
	.custoEstimadoOut(custoEstimadoS4), 
	
	.address(addressS3),
	.addressOut(addressS4),
	
	.direcao(direcaoS3),
	.direcaoOut(direcaoS4),
	
	.aberto(abertoS3),
	.abertoOut(abertoS4),
	
	.valor(valorS3),
	.valorOut(valorS4),
	
	.x(xS3),
	.xOut(xS4),
	
	.y(yS3),
	.yOut(yS4),
	
	.candidato(candidatoS3),
	.candidatoOut(candidatoS4),
	
	.dx(dx),
	.dxOut(dxS4),
		
	.dy(dy),
	.dyOut(dyS4),
		
	.lastUse(lastUseS3),
	.lastUseOut(lastUseS4)
);


assign dx_ = (dxS4[linear - 1]) ? !dxS4 : dxS4;

assign dy_ = (dyS4[linear - 1]) ? !dyS4 : dyS4;

assign distanciaTotal = dx_ + dy_;

S4 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
s4 (

	.clock(clock),
	.reset(reset),
	
	.custoAcumulado(custoAcumuladoS4),
	.custoAcumuladoOut(custoAcumuladoS5), 
	
	.custoEstimado(custoEstimadoS4),
	.custoEstimadoOut(custoEstimadoS5), 
	
	.address(addressS4),
	.addressOut(addressS5),
	
	.direcao(direcaoS4),
	.direcaoOut(direcaoS5),
	
	.aberto(abertoS4),
	.abertoOut(abertoS5),
	
	.valor(valorS4),
	.valorOut(valorS5),
	
	.distanciaTotal(distanciaTotal),
	.distanciaTotalOut(distanciaTotalS5),
	
	.x(xS4),
	.xOut(xS5),
	
	.y(yS4),
	.yOut(yS5),
	
	.candidato(candidatoS4),
	.candidatoOut(candidatoS5),
		
	.lastUse(lastUseS4),
	.lastUseOut(lastUseS5)
	
);
 
assign novoCustoEstimado = custoEstimadoS5 + distanciaTotalS5;
 
assign dataToMalha = {
		 xS5, yS5,
         valorS5,
         direcaoS5, //pai
         custoAcumuladoS5,
         novoCustoEstimado,
         1'b1};

assign writeMalha = candidatoS5;
assign writeFIFO = candidatoS5;
assign dataFIFO = {abertoS5, addressS5};

assign lastUseOutput = lastUseS5;

endmodule