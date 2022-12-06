module top(
    input  wire clk,
    input  wire reset_n,
    output reg [1:0] ledout
);

wire reset;
reg [9:0] kilo_cnt;
reg [9:0] mega_cnt;
reg [6:0] hundred_cnt;

assign reset = ~reset_n;

always @(posedge clk) begin
    if(reset) begin
        kilo_cnt <= 0;
        mega_cnt <= 0;
        hundred_cnt <= 0;
        ledout <= 2'b0;
    end else if (kilo_cnt == 999) begin
        kilo_cnt <= 0;
        if (mega_cnt == 999) begin
            mega_cnt <= 0;
            if (hundred_cnt == 99) begin
                hundred_cnt <= 0;
                ledout = ~ledout;
            end else begin
                hundred_cnt <= hundred_cnt + 1;
            end
        end else begin
            mega_cnt <= mega_cnt + 1;
        end
    end else begin
        kilo_cnt <= kilo_cnt + 1;
    end

    
end    

endmodule