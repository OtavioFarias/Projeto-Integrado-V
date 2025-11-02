`timescale 1ns/1ps

module tb();

  // parâmetros
  localparam int TamanhoMalha = 8;
  localparam int tamanhoDistancia = 8;

  // sinais
  logic clock;
  logic reset;
  logic novoDado;

  logic [tamanhoDistancia-1:0] posicaoAtualnoEixoX;
  logic [tamanhoDistancia-1:0] posicaoAtualnoEixoY;

  reg [1:0] malha [TamanhoMalha*TamanhoMalha];

  wire operacaoFinalizada;
  wire [tamanhoDistancia-1:0] destinoX;
  wire [tamanhoDistancia-1:0] destinoY;

  logic [3:0] enable;
  // DUT
  distancias #(
    .TamanhoMalha(TamanhoMalha),
    .tamanhoDistancia(tamanhoDistancia)
  ) dut (
    .clock(clock),
    .reset(reset),
    .posicaoAtualnoEixoX(posicaoAtualnoEixoX),
    .posicaoAtualnoEixoY(posicaoAtualnoEixoY),
    .novoDado(novoDado),
    .malha(malha),
    .operacaoFinalizada(operacaoFinalizada),
    .destinoX(destinoX),
    .destinoY(destinoY),
    .enable(enable)
  );

  // clock 10ns
  initial clock = 0;
  always #5 clock = ~clock;

  // tarefa para preencher a malha
  task preencher_malha;
    int i;
    begin
      for (i = 0; i < TamanhoMalha*TamanhoMalha; i++) begin
        malha[i] = 2'b00; // livre
      end
      // marcar um alvo (por exemplo 3 = candidato)
      malha[2 + 3*TamanhoMalha] = 2'b11; // outro ponto de interesse
    end
  endtask

  // teste principal
  initial begin
    $display("=== Início da simulação ===");
    reset = 1;
    novoDado = 0;
    
    //posicaoAtualnoEixoX = 0;
    //posicaoAtualnoEixoX = TamanhoMalha - 1;
   	 
    //posicaoAtualnoEixoY = 0; 
    //posicaoAtualnoEixoY = TamanhoMalha - 1;
    
    //1000 - direitaFrente ok, 0100 - esquerdaFrente ok, 
    //0010 - direitaTras ok ok, 0001 - esquerdaTras ok ok
	
	posicaoAtualnoEixoX = 3;
    posicaoAtualnoEixoY = 3;
	
    enable = 4'b1111;
    preencher_malha();

    #20 reset = 0;

    // iniciar operação
    #10 novoDado = 1;
    #10 novoDado = 0;

    // aguardar até que DUT finalize (em um projeto real teria um flag)
    repeat (2000) @(posedge clock);
	
    $display("=== Resultado ===");
    $display("Destino encontrado: X=%0d Y=%0d", destinoX, destinoY);
    $display("Operacao finalizada: %0b", operacaoFinalizada);
	
    #50 $finish;
  end

  // monitorar sinais
  
  /*
  initial begin
    $monitor("T=%0t | stage=? | raio=? | candX=%0d candY=%0d destinoX=%0d destinoY=%0d direitaAcabou=%b",
              $time, dut.raio, dut.coordenadaCandidatoDireitaFrenteX,
              dut.coordenadaCandidatoDireitaFrenteY,
              destinoX, destinoY, dut.direitaAcabou);
  end
*/
/*
	always @(posedge clock) begin
    	if(!reset) begin
            $display("=== Malha Atualizada ===");
            for(int i=0; i < TamanhoMalha; i=i+1) begin
                for(int j=TamanhoMalha - 1; j >= 0; j=j-1) begin
                    $write("%0d ", malha[i*TamanhoMalha+j]); // [x][y]
                end
                $write("\n");
            end
            $display("========================");
        end
    end
*/
endmodule
