program out2asm;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,classes;

  var namein,nameout,s:string;
  fi:TFileStream;
  fo:TStringList;
  b,k:byte;
begin
  try
    { TODO -oUser -cConsole Main : Insérer du code ici }
    if ((ParamCount=0) or (ParamCount>2)) then exit;
    nameout:='';
    namein:=ParamStr(1);
    s:=ExtractFileExt(namein);

    if ParamCount=2 then nameout:=ParamStr(2) else nameout:=namein;
    if s='' then namein:=namein+'.out';
    s:=ExtractFileExt(nameout);
    if s='' then nameout:=nameout+'.s';
    fi:=TFileStream.Create(namein,fmOpenRead);
    fo:=TStringList.Create;
    try
      k:=0;
      fo.Add('_AYData');
      fi.Position:=0;
      while fi.Position<fi.Size do
      begin
        if k=0 then s:='    .byte   ';
        fi.Read(b,1);

        if k<>1 then
        begin
           s:=s+'$'+IntToHex(b,2);
           if k<>3 then s:=s+',';
        end;

        inc(k);
        if k=4 then
        begin
          k:=0;
          fo.Add(s);
        end;
      end;
      fo.Add('_AYData_End');
      fo.Add('_AYData_Size    .word (_AYData_End - _AYData)');
      fo.SaveToFile(nameout);
    finally
      fi.Free;
      fo.Free;
    end;
{static unsigned char getb(void)
{
    int c = fgetc(fi);
    if(-1==c)
        quit = 1;
    return (unsigned char)(0xFF & c);
}

  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
