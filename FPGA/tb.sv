`timescale 1ns/1ps

module tb;

    // Parâmetros
    parameter TamanhoMalha = 8;
    parameter tamanhoDistancia = 4; // bits para posição/distância

    // Sinais do DUT
    logic clock;
    logic reset;

    logic [tamanhoDistancia-1:0] posicaoAtualnoEixoX;
    logic [tamanhoDistancia-1:0] posicaoAtualnoEixoY;
    logic direcaoAtual;
    logic [tamanhoDistancia-1:0] distanciaFrente;
    logic [tamanhoDistancia-1:0] distanciaDireita;
    logic [tamanhoDistancia-1:0] distanciaEsquerda;
    logic novoDado;

    logic [1:0] malha[TamanhoMalha-1:0][TamanhoMalha-1:0];
    logic operacaoFinalizada;

    // Instancia o DUT
    mapas #(
        .TamanhoMalha(TamanhoMalha),
        .tamanhoDistancia(tamanhoDistancia)
    ) dut (
        .clock(clock),
        .reset(reset),
        .posicaoAtualnoEixoX(posicaoAtualnoEixoX),
        .posicaoAtualnoEixoY(posicaoAtualnoEixoY),
        .direcaoAtual(direcaoAtual),
        .distanciaFrente(distanciaFrente),
        .distanciaDireita(distanciaDireita),
        .distanciaEsquerda(distanciaEsquerda),
        .novoDado(novoDado),
        .malha(malha),
        .operacaoFinalizada(operacaoFinalizada)
    );

    // Clock de 10 ns
    initial clock = 0;
    always #5 clock = ~clock;

    // Reset e estímulos
    initial begin
        reset = 1;
        novoDado = 1;
        posicaoAtualnoEixoX = 3;
        posicaoAtualnoEixoY = 0;
        direcaoAtual = 1;
        distanciaFrente = 1;
        distanciaDireita = 2;
        distanciaEsquerda = 1;

        #20 reset = 0; // libera reset

		/*

        // Espera operação finalizar antes de mandar dados
        repeat (10) begin
            wait(operacaoFinalizada == 1);
            enviar_novo_dado();
            #10;
        end
		*/
		
		wait(operacaoFinalizada == 0);
		wait(operacaoFinalizada == 1);
		novoDado = 0;
		
		#200
		
        $finish;
    end

    // Tarefa para enviar dados novos
    task enviar_novo_dado;
        begin
            posicaoAtualnoEixoX = $urandom_range(0, TamanhoMalha-1);
            posicaoAtualnoEixoY = $urandom_range(0, TamanhoMalha-1);
            direcaoAtual = $urandom_range(0,1);
            distanciaDireita = $urandom_range(1, TamanhoMalha/2);
            distanciaEsquerda = $urandom_range(1, TamanhoMalha/2);
            novoDado = 1;
            @(posedge clock);
            novoDado = 0;
        end
    endtask

    // Monitor para imprimir malha quando operação finalizada
    always @(posedge clock) begin
        if(operacaoFinalizada) begin
            $display("=== Malha Atualizada ===");
            for(int i=0; i<TamanhoMalha; i=i+1) begin
                for(int j=0; j<TamanhoMalha; j=j+1) begin
                    $write("%0d ", malha[j][i]); // [x][y]
                end
                $write("\n");
            end
            $display("========================");
        end
    end

endmodule
