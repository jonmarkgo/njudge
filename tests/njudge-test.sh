#!/usr/bin/perl -w

# $Log$
# Revision 1.3  2004/01/18 01:12:49  millis
# Added credit to Jeff Smith
#
# Revision 1.2  2004/01/18 01:11:05  millis
# Make diff work via Unix 'diff' command
#
# Revision 1.1  2004/01/18 00:56:41  millis
# First version (from Jeff Smith <smith@pobox.com>)
#

use strict;
use File::Copy;
use Cwd;

my $stat = 0;
my $dip = "dip -A -a ";
my $outdir = "output";
my $indir = "input";
my $baselines = "baseline";
my $heredir = cwd();
my $dstat = 0;

# Paramter $1 should be the directory to call
# ./dip from, defaulting to /home/judge.
(@ARGV > 1) && die("usage: $0 [run-dir]\n");
my $dir = $ARGV[0] ? $ARGV[0] : "/home/judge";
#chdir($dir) || die("unable to chdir to $dir: $!\n");
#(-d $outdir) || mkdir($outdir,0755) || die("unable to mkdir $outdir: $!\n");
#(-d $baselines) || mkdir($baselines,0755) || die("unable to mkdir $baselines: $!\n");

#this stuff to make sure all data files are reset 
chdir("..");
system("/bin/rm $dir/dip.master  $dir/dip.addr  $dir/dip.whois $dir/dip.ded");
system("rm -rf $dir/D*");
$stat = system("make install >& /dev/null");
if ($stat)
{
     print("\nMake install returned error: check source code!\n\n");
     exit(1);
}
my $master = "$dir/dip.master";

chdir($heredir);

#Create new smail to save emails too.
system("cp -p $dir/smail $dir/smail.old");
system ("cp -p $heredir/smail-test $dir/smail");

# For each file $FILE.in in the input subdirectory
# (ordered alphabetically)
chdir($indir);
my @files = sort(<*.in>);
chdir("..");
my ($infile,$inpfile,$test,$text,$output,$baseline, $datetime, $masterbase, $masteroutput);

foreach $infile (@files)
{
	$test = (split(/\./,$infile))[0];
	$text = "$indir/$test.txt";
	$output = "$outdir/$test.out";
	$baseline = "$baselines/$test.out";
	$inpfile = "$indir/$infile";
	$datetime = "'1 Jan 2004'";


    # display the file $FILE.txt (i.e. cat $FILE.txt)
   
    unless (-e $text)
    {
        system("echo $text >> $text");
    } 
    
    print join("",slurp($text));
#    <STDIN>;
#Zap output mail file
    unlink("$dir/smail.out");
# call ./dip executable, passing the relevant $FILE.in file
        $dstat = system("$dir/$dip -C $dir -i $heredir/$inpfile -T $datetime >& $heredir/$output");

	if ($dstat) 
	{
	    print("Dip returned error!");
	    print("\n$dir/$dip -C $dir -i $heredir/$inpfile -T $datetime >& $heredir/$output\n");
	    exit(1);
	}

#Add the description file to the end of the output (which should probably be empty if all worked ok)
system("cat $text >> $heredir/$output");

#append the email sent to outfile
system ("cat $dir/smail.out >> $heredir/$output");

	# compare any output file with its equivalents in baseline subdirectory
	# (dip.$FILE.output), or copy if non-existent.
	# If match fails, must later exit with non-zero exit code.

	unless (-e $baseline)
	{
		copy($output,$baseline);
	}

	if (system("diff $output $baseline")) 
	{
		$stat += 1;
	}
# Save dip.master file for further reference

        $masterbase = "$baselines/$test.dip-master";
	$masteroutput = "$outdir/$test.dip-master";
	unless (-e $masterbase)
	{
		copy($master, $masterbase);
	}
	copy($master, $masteroutput);
	if (system("diff $masteroutput $masterbase"))
	{
	     $stat += 1;
	}

#	cmpfiles($output,$baseline);
}

print ("Tests finished with $stat errors\n");
#restore old smail
system ("chmod +w $dir/smail");
system ("cp -p $dir/smail.old $dir/smail");
if ($stat) 
{
    exit(1);
}
exit(0);

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
