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

wire [tamanhoDados - 1:0] data00 = data[0];
wire [tamanhoDados - 1:0] data01 = data[1];
wire [tamanhoDados - 1:0] data02 = data[2];
wire [tamanhoDados - 1:0] data03 = data[3];
wire [tamanhoDados - 1:0] data04 = data[4];
wire [tamanhoDados - 1:0] data05 = data[5];
wire [tamanhoDados - 1:0] data06 = data[6];
wire [tamanhoDados - 1:0] data07 = data[7];
wire [tamanhoDados - 1:0] data08 = data[8];
wire [tamanhoDados - 1:0] data09 = data[9];
wire [tamanhoDados - 1:0] data10 = data[10];
wire [tamanhoDados - 1:0] data11 = data[11];
wire [tamanhoDados - 1:0] data12 = data[12];
wire [tamanhoDados - 1:0] data13 = data[13];
wire [tamanhoDados - 1:0] data14 = data[14];
wire [tamanhoDados - 1:0] data15 = data[15];
wire [tamanhoDados - 1:0] data16 = data[16];
wire [tamanhoDados - 1:0] data17 = data[17];
wire [tamanhoDados - 1:0] data18 = data[18];
wire [tamanhoDados - 1:0] data19 = data[19];
wire [tamanhoDados - 1:0] data20 = data[20];
wire [tamanhoDados - 1:0] data21 = data[21];

endmodule
