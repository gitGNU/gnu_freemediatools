#!/usr/bin/perl

# Replaces strings

$file = $ARGV[0];
$outFilename = $ARGV[1];
$searchPattern = $ARGV[2];
$replacePattern = $ARGV[3];
#print "$outFilename\n";


open(INFILE, $file);
open(OUTFILE, ">" . $outFilename);

while (<INFILE>)
{
	#print "$searchPattern\n";
	#$found = m/$searchPattern/;
	$found = 0;

	if ($found)
	{
		print "$file:\n";
		print $_;
	}

	s/$searchPattern/$replacePattern/g;
	#s#(^//.*\*\*)$#\1//#;


	if ($found)
	{
		print $_;
	}

	print OUTFILE $_;
}
