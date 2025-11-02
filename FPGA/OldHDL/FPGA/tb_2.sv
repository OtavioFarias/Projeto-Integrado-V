`timescale 1ns/1ps

module tb_2;

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
        novoDado = 0;
        posicaoAtualnoEixoX = 0;
        posicaoAtualnoEixoY = 0;
        direcaoAtual = 0;
        distanciaFrente = 1;
        distanciaDireita = 2;
        distanciaEsquerda = 1;

        #20 reset = 0; // libera reset

        // envia vários dados ao DUT
        repeat (10) begin
            wait(operacaoFinalizada == 1);  // espera operação anterior terminar
            enviar_novo_dado();
        end

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

    // Função para imprimir a malha em ASCII
    task print_malha;
        begin
            $display("Malha atual:");
            for(int y = TamanhoMalha-1; y >= 0; y=y-1) begin
                for(int x = 0; x < TamanhoMalha; x=x+1) begin
                    case(malha[x][y])
                        2'b00: $write(". "); // desconhecido
                        2'b01: $write("O "); // livre
                        2'b10: $write("X "); // ocupado
                        default: $write("? ");
                    endcase
                end
                $write("\n");
            end
            $display("------------------------");
        end
    endtask

    // Monitor animado: imprime a malha a cada ciclo
    always @(posedge clock) begin
        // mostra sempre que a operação terminou ou se o robô está marcando
        if(operacaoFinalizada) begin
            $display("=== Operação Finalizada ===");
            print_malha();
        end
        else begin
            // opcional: pode imprimir a cada passo intermediário
            // print_malha();
        end
    end

endmodule
