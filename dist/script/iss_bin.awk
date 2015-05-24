BEGIN {
  PART1="Source: \""
  PART2="\"; DestDir: \"{app}\\"
  PART3="\"; Flags: ignoreversion touch"
}

{
  match($1, /.*\\/)
  DIR=substr($1, 1, RLENGTH-1)
  printf("%s%s%s%s%s\n", PART1, $1, PART2, DIR, PART3);
}
