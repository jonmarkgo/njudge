#!/usr/bin/perl
#
#  Invoked as smail file subject address
#
# TODO allow additional addresses on command line

($filename, $subject, $address) = @ARGV;
$sendmail = "/usr/sbin/sendmail";
$jcode = $ENV{'JUDGE_CODE'} || 'XXXX';

$from    = '"' . $jcode . ' Diplomacy Adjudicator" <judge@' . lc($jcode)
           . '.judge.igo.org>';
$replyto = 'judge@' . lc($jcode) . '.judge.igo.org';

open(FILE, $filename);
@file = <FILE>;
close(FILE);

open(DIPMSG, "dip.msg");
@dipmsg = <DIPMSG>;
close(DIPMSG);

open(MAILLOG, ">>mail.log");
print MAILLOG scalar(gmtime), "\n";
print MAILLOG "===== mail sent to $address/$subject -> $jcode\n";
print MAILLOG @file[0..9];
close(MAILLOG);

open(SENDMAIL, "|$sendmail $address");
print SENDMAIL <<"EOM";
To: $address
Reply-To: $replyto
From: $from
Subject: $subject

EOM
print SENDMAIL @dipmsg, @file;
close(SENDMAIL);

unlink("dip.xfail");
