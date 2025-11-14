module Malha (

    input clock,
    input reset,

	input cacheReq,
    input [31:0] value,
    input read, write,
    input [31:0] address,

    output reg [31:0] outData,
    output reg ready,
    output reg hit,

);

    localparam IDLE = 2'b00,
               WRITE_BACK = 2'b01,
               UPDATE_CACHE = 2'b11;

    localparam READ  = 1'b1,
               WRITE = 1'b0;

    parameter CacheTam = 32;

    reg [1:0] stage;

    reg [31:0] data[0:CacheTam-1];
    reg valid[0:CacheTam-1];
    reg dirty[0:CacheTam-1];
    reg [24:0] tagArray[0:CacheTam-1];

    wire [4:0] index = address[6:2];
    wire [24:0] tag = address[31:7];
    wire [1:0] request;

	//DEBUG
	wire [24:0] tagDebug;
	wire validDebug;
	assign validDebug = valid[index];
	assign tagDebug = tagArray[index];

    always_ff @(posedge clock, posedge reset) begin
        if (reset) begin
            for (integer i = 0; i < CacheTam; i = i + 1) begin
                valid[i]    <= 1'b0;
                tagArray[i] <= 1'b0;
                dirty[i]    <= 1'b0;
            end

            writeRAM <= 1'b0;
            stage <= IDLE;
            readRAM <= 0;
            ready <= 1;
            addressToRAM <= 0;
            hit <= 0;

        end else begin
            case (stage)

                IDLE: begin
                
		            		if(cacheReq) begin 
		                case (read)

		                    READ: begin
		                        if (valid[index] && tagArray[index] == tag) begin
		                             outData <= data[index];
		                             stage <= IDLE;
		                        		ready <= 1;
		                        		hit <= 1;
		                        	
		                        end else begin
		                            ready <= 0;
		                            hit <= 0;

		                            if (dirty[index]) begin
		                                dirty[index] <= 1'b0;
		                                writeRAM <= 1'b1;
		                                stage <= WRITE_BACK;
		                                addressToRAM <= {tagArray[index], index};

		                                if (byteExtend) begin
		                                    valueRAM[7:0] = data[index][7:0];
		                                end else begin
		                                    if (halfExtend) begin
		                                        valueRAM[15:0] = data[index][15:0];
		                                    end else begin
		                                        valueRAM = data[index];
		                                    end
		                                end
		                            end else begin
		                                addressToRAM <= address;
		                                readRAM <= 1'b1;
		                                stage <= UPDATE_CACHE;
		                            end
		                        end
		                    end

		                    WRITE: begin
		                        if (valid[index] && tagArray[index] == tag) begin
		                            data[index] <= value;
		                            dirty[index] <= 1'b1;
		                            stage <= IDLE;
		                            ready <= 1;
		                            hit <= 1;
		                            
		                        end else begin
		                            ready <= 0;
		                            hit <= 0;

		                            if (dirty[index]) begin
		                                dirty[index] <= 1'b0;
		                                stage <= WRITE_BACK;
		                                writeRAM <= 1'b1;
		                                addressToRAM <= {tagArray[index], index};

		                                if (byteExtend) begin
		                                    valueRAM[7:0] = data[index][7:0];
		                                end 
		                                else begin
		                                    if (halfExtend) begin
		                                        valueRAM[15:0] = data[index][15:0];
		                                    end else begin
		                                        valueRAM = data[index];
		                                    end
		                                end
		                            end else begin
		                                readRAM <= 1'b1;
		                                addressToRAM <= address;
		                                stage <= UPDATE_CACHE;
		                            end
		                        end
		                    end
	   
		                endcase
		            end
		            else begin
		            
				        	stage <= IDLE;
						hit <= 0;
		            
		            end
                end

                WRITE_BACK: begin
                    if (dataUsingRAM) begin
                        stage <= UPDATE_CACHE;
                    end else begin
                        stage <= WRITE_BACK;
                    end
                end

                UPDATE_CACHE: begin
                    if (ramReady) begin
                        
                        outData <= outRAM;
                        stage <= IDLE;
                        ready <= 1;

                        if (writeRAM) begin
                            writeRAM <= 0;
                        end else begin
                            valid[index] <= 1;
                            tagArray[index] <= tag;
                            data[index] <= outRAM;
                            readRAM <= 1'b0;
                     
                        end
                    end else begin
                        stage <= UPDATE_CACHE;
                    end
                end

            endcase
        end
    end

    
    wire [31:0] data00 = data[0];
    wire [31:0] data01 = data[1];
    wire [31:0] data02 = data[2];
    wire [31:0] data03 = data[3];
    wire [31:0] data04 = data[4];
    wire [31:0] data05 = data[5];
    wire [31:0] data06 = data[6];
    wire [31:0] data07 = data[7];
    wire [31:0] data08 = data[8];
    wire [31:0] data09 = data[9];
    wire [31:0] data10 = data[10];
    wire [31:0] data11 = data[11];
    wire [31:0] data12 = data[12];
    wire [31:0] data13 = data[13];
    wire [31:0] data14 = data[14];
    wire [31:0] data15 = data[15];
    wire [31:0] data16 = data[16];
    wire [31:0] data17 = data[17];
    wire [31:0] data18 = data[18];
    wire [31:0] data19 = data[19];
    wire [31:0] data20 = data[20];
    wire [31:0] data21 = data[21];
    wire [31:0] data22 = data[22];
    wire [31:0] data23 = data[23];
    wire [31:0] data24 = data[24];
    wire [31:0] data25 = data[25];
    wire [31:0] data26 = data[26];
    wire [31:0] data27 = data[27];
    wire [31:0] data28 = data[28];
    wire [31:0] data29 = data[29];
    wire [31:0] data30 = data[30];
    wire [31:0] data31 = data[31];
    

endmodule
