module Malha #(parameter int TamanhoMalha = 20, parameter int tamanhoAddr = 8, parameter int tamanhoDados = 8)(

    input clock,
    input reset,

    input [tamanhoDados - 1:0] value,
    input read, write,
    input [tamanhoAddr - 1:0] address,

    output reg [tamanhoDados - 1:0] outData,
    output reg dataReady
    
);

localparam RESTO = tamanhoDados - 2 * $clog2(TamanhoMalha); 
localparam LINEAR = $clog2(TamanhoMalha);

localparam READ  = 1'b1,
           WRITE = 1'b0;

reg [tamanhoDados - 1:0] data[0:TamanhoMalha*TamanhoMalha-1];

always_ff @(posedge clock, posedge reset) begin
    if (reset) begin

        for (int y = 0; y < TamanhoMalha; y++) begin
            for (int x = 0; x < TamanhoMalha; x++) begin
                
                data[x + TamanhoMalha*y] <= { x[LINEAR-1:0], y[LINEAR-1:0], {RESTO{1'b0}}};
				
            end
        end
        
        dataReady <= 0;

    end else begin

        if(read) begin

			outData <= data[address];
			dataReady <= 1;

        end

        if(write) begin
               
            data[address] <= value;
            dataReady <= 1;
   
        end
        
        if(!(write || read)) begin
               
        		dataReady <= 0;
   
        end
        
    end
end

endmodule
