module Top(

	input clock,
	input reset,
	
	input dadosEntrada,
	
	output reg dadosSaida

);

parameter TamanhoMalha = 9;
parameter tamanhoDistancia = 4; // bits para posição/distância

mapas #(
    .TamanhoMalha(TamanhoMalha),
    .tamanhoDistancia(tamanhoDistancia)
) UnidadeDeMapeamento (

    .clock(clock),
    .reset(reset),
    .posicaoAtualnoEixoX(posicaoAtualnoEixoX),
    .posicaoAtualnoEixoY(posicaoAtualnoEixoY),
    .direcaoAtual(direcaoAtual),
    .distanciaDireita(distanciaDireita),
    .distanciaEsquerda(distanciaEsquerda),
    .novoDado(novoDado),
    .malha(malha),
    .operacaoFinalizada(operacaoFinalizada)
    
);


distancias #(   
	.TamanhoMalha(TamanhoMalha),
	.tamanhoDistancia(tamanhoDistancia)
) UnidadeBuscaNovoTrajeto (

    .clock(clock),
    .reset(reset),
    
    .posicaoAtualnoEixoX(posicaoAtualnoEixoX),
    .posicaoAtualnoEixoY(posicaoAtualnoEixoY),
    
    .novoDado(novoDado),
    .malha(malha),
    .operacaoFinalizada(operacaoFinalizada)
    
);

sync_fifo #(   
	.DEPTH(TamanhoMalha),
	.DWIDTH(tamanhoDistancia*2) //armazenar o par de coordenads (x,y)
) filaBFS(


        .rstn(),               // Active high reset
        .clk(),                // Clock
        .wr_en(), 				// Write enable
        .rd_en(), 				// Read enable
        .din(), 				// Data written into FIFO
        .dout() 				// Data read from FIFO
        //empty, 				// FIFO is empty when high
    	//full 				// FIFO is full when high
);

sync_fifo #(   
	.DEPTH(TamanhoMalha),
	.DWIDTH(tamanhoDistancia*2) //armazenar o par de coordenads (x,y)
) filaTrajeto(


        .rstn(),               // Active high reset
        .clk(),                // Clock
        .wr_en(), 				// Write enable
        .rd_en(), 				// Read enable
        .din(), 				// Data written into FIFO
        .dout() 				// Data read from FIFO
        //empty, 				// FIFO is empty when high
    	//full 				// FIFO is full when high
);

endmodule