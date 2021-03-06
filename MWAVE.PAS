program makewave;

{
 ***************************************************************************
 *                                                                         *
 *            Wave maker for SoundBlaster 1.5 by HaJott                    *
 *                                                                         *
 ***************************************************************************
   Discription of data types :

   type
      chunk = record
        main               : string[4];             = RIFF
        main_length        : array[0..3] of byte;   = length of file
        chunk_type         : string[4];             = WAVE
        sub_chunk          : string[4];             = fmt_ :_=space=ASCII 32
        sub_chunk_length   : array[0..3] of byte;   = always 16 bytes
                                                      don't know why !
        format             : word;                  = always 1 for PCM mode
                             every SoundBlaster works with format = 1
        modus              : word;                  1 = mono, 2 = stereo
        sample_freq        : array[0..3] of byte;   11.025 kHz = $2B11
                                                    22.050 kHz = $5622
                                                    44.100 kHz = $AC44
        bytes_per_sec      : array[0..3] of byte;   as it is
        bytes_per_sample   : word;                  1 = 8 Bit, 2 = 16 Bit
        bits_per_sample    : word;                  only 8, 12 or 16
        data_chunk         : string[4];             = data
        data_length        : array[0..3] of byte;   length of data block
      end;

}

{$N+}

 type
        w                  = word;
        dw                 = array[0..3] of byte;
{       w                  = array[0..1] of byte;}
{       st                 = string[4]; }
        dat                = array[1..16384] of byte;

      recordType = record
        main               : dw{st} ;
        main_length        : dw;
        chunk_type         : dw{st};
        sub_chunk          : dw{st};
        sub_chunk_length   : dw;
        format             : w;
        modus              : w;
        sample_freq        : dw;
        bytes_per_sec      : dw;
        bytes_per_sample   : w;
        bits_per_sample    : w;
        data_chunk         : dw{st};
        data_length        : dw;
        rf                 : dat;
      end;
    datafile = recordType;

var i,j,k,maxdat:integer;
    datei1:file of datafile;
    ri : byte;
    x:extended;
    stepx : real;
    code:word;
    p:pointer;
    lenght : integer;
    chunk                :  recordType;
    step                 :  Integer;
    bol                  :  Boolean;
    wo                   :  word;
    sinortri             :  char;



procedure initall;
begin
  x:=0; code:=0;i:=0;k:=0;j:=0;bol:=true;
  chunk.main[0]:=ord('R');
  chunk.main[1]:=ord('I');
  chunk.main[2]:=ord('F');
  chunk.main[3]:=ord('F');
  chunk.main_length[0]:=45;
  chunk.main_length[1]:=64;
  chunk.main_length[2]:=0;
  chunk.main_length[3]:=0;
  chunk.chunk_type[0]:=ord('W');
  chunk.chunk_type[1]:=ord('A');
  chunk.chunk_type[2]:=ord('V');
  chunk.chunk_type[3]:=ord('E');
  chunk.sub_chunk[0]:=ord('f');
  chunk.sub_chunk[1]:=ord('m');
  chunk.sub_chunk[2]:=ord('t');
  chunk.sub_chunk[3]:=ord(' ');
  chunk.sub_chunk_length[0]:=16;
  chunk.sub_chunk_length[1]:=0;
  chunk.sub_chunk_length[2]:=0;
  chunk.sub_chunk_length[3]:=0;
  chunk.format:=1;
  chunk.modus:=1;
  chunk.sample_freq[0]:=17;
  chunk.sample_freq[1]:=43;
  chunk.sample_freq[2]:=0;
  chunk.sample_freq[3]:=0;
  chunk.bytes_per_sec[0]:=68;
  chunk.bytes_per_sec[1]:=172;
  chunk.bytes_per_sec[2]:=0;
  chunk.bytes_per_sec[3]:=0;
  chunk.bytes_per_sample:=4;
  chunk.bits_per_sample:=8;
  chunk.data_chunk[0]:=ord('d');
  chunk.data_chunk[1]:=ord('a');
  chunk.data_chunk[2]:=ord('t');
  chunk.data_chunk[3]:=ord('a');
  chunk.data_length[0]:=0;
  chunk.data_length[1]:=40;
  chunk.data_length[2]:=0;
  chunk.data_length[3]:=0;
for j:=1 to 16384 do chunk.rf[i]:=0;
end;

begin
initall;
writeln;
writeln (' Wave maker for SoundBlaster 1.5 by HaJott ');
writeln;
writeln (' Enter number in () to create wave ');
write ('Pulse wave (2), Sinus wave (1), triangle wave (0) : ');
readln(sinortri);
case sinortri of
('1') :
      begin
           stepx:=0.4;
               {*** stepx makes the frequency, 0.4=low 0.8=high ***}
           writeln;
           writeln ('Make Sinus ');
           for i:=1 to 16384 do
             begin
              chunk.rf[i]:=abs(round(127+127*sin(x)));
              {this makes a sinus wave between 0 and 254 (8bit)}
              x:=x+stepx;
             end; {for i:=1 to 16384 do}
      end;
('2') :
      begin
           i:=0;
           step:=16;
            {*** step makes the frequency, step=64 means low step=16 high}
           maxdat:=16384 div (2*step);
           writeln;
           writeln ('Make Pulse ');
           for k:=1 to maxdat do
            begin
             for j:=1 to step do begin inc(i); chunk.rf[i]:=0;end;
             for j:=1 to step do begin inc(i); chunk.rf[i]:=255;end;
             {*** makes a pulse wave between 0 and 255 (8bit) ***}
            end; {for k:=1 to 256 do}
       end
else
       begin {makes a triangle wave between 0 an 254 (8bit)}
         writeln;
         writeln ('Make Triangle ');
         step:=8; {step makes the frequency, step=8 means low step=16 high}
         while (i<=16384) do
           begin
             if chunk.rf[i]>=(255-step) then begin step:=-step;bol:=false;end;
             if chunk.rf[i]<=(step) then begin step:=abs(step);bol:=true;end;
             inc(i);
             chunk.rf[i]:=chunk.rf[i-1]+step;
           end;
       end;
end;{case}

writeln ('Size of complete Datafile : ',SizeOf(chunk));
writeln ('Size of wave data : ',SizeOf(chunk.rf));
  chunk.main_length[0]:=lo(SizeOf(chunk));
  chunk.main_length[1]:=hi(SizeOf(chunk));
  chunk.data_length[0]:=lo(SizeOf(chunk.rf));
  chunk.data_length[1]:=hi(SizeOf(chunk.rf));
assign(datei1,'c:\mmos2\sounds\aaa.wav');
rewrite(datei1);
write (datei1,datafile(chunk));
close(datei1);
end.
