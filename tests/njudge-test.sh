#!/usr/bin/perl

# $Log$
# Revision 1.1  2004/01/18 00:56:41  millis
# First version
#

use strict;
use File::Copy;

my $stat = 0;
my $dip = "dip -A -a";
my $outdir = "output";
my $indir = "input";
my $baselines = "baseline";
#This should be changed to get absolute path where this script runs from ($0)
my $heredir = "/home/judge/j-code/njudge/tests";

# Paramter $1 should be the directory to call
# ./dip from, defaulting to /home/judge.
(@ARGV > 1) && die("usage: $0 [run-dir]\n");
my $dir = $ARGV[0] ? $ARGV[0] : "/home/judge";
#chdir($dir) || die("unable to chdir to $dir: $!\n");
#(-d $outdir) || mkdir($outdir,0755) || die("unable to mkdir $outdir: $!\n");
#(-d $baselines) || mkdir($baselines,0755) || die("unable to mkdir $baselines: $!\n");

#this stuff to make sure all data files are reset 
chdir("..");
system("rm $dir/dip.master");
system("make install >& /dev/null");
chdir($heredir);

#Create new smail to save emails too.
system("cp -p $dir/smail $dir/smail.old");
system ("cp -p $heredir/smail-test $dir/smail");

# For each file $FILE.in in the input subdirectory
# (ordered alphabetically)
chdir($indir);
my @files = sort(<*.in>);
chdir("..");
my ($infile,$inpfile,$test,$text,$output,$baseline);
foreach $infile (@files)
{
	$test = (split(/\./,$infile))[0];
	$text = "$indir/$test.txt";
	$output = "$outdir/$test.out";
	$baseline = "$baselines/$test.out";
	$inpfile = "$indir/$infile";


    # display the file $FILE.txt (i.e. cat $FILE.txt)
    print join("",slurp($text));
# call ./dip executable, passing the relevant $FILE.in file
	system("$dir/$dip -C $dir -i $heredir/$inpfile >& $heredir/$output");
#append the email sent to outfile
system ("cat $dir/smail.out >> $heredir/$output");

	# compare any output file with its equivalents in baseline subdirectory
	# (dip.$FILE.output), or copy if non-existent.
	# If match fails, must later exit with non-zero exit code.

	unless (-e $baseline)
	{
		copy($output,$baseline);
	}

	$stat += system("diff $output $baseline");
#	cmpfiles($output,$baseline);
}

#restore old smail
system ("chmod +w $dir/smail");
system ("cp -p $dir/smail.old $dir/smail");
exit($stat);

sub cmpfiles
{
	my ($file1,$file2) = @_;
	my @file1 = slurp($file1);
	my @file2 = slurp($file2);

	if ($#file1 != $#file2)
	{
		return abs($#file1 - $#file2);
	}

	my ($i,$val);
	for ($i = 0; $i < @file1; ++$i)
	{
		$val += abs($file1[$i] cmp $file2[$i]);
	}
	return $val;
}

sub slurp
{
	my ($file) = @_;

	open(FILE,$file) || die("unable to open $file for reading: $!\n");
	my @lines = <FILE>;
	close(FILE);
	return @lines;
}
