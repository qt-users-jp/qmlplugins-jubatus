%modules = ( # path to module name map
    "QtJubatus" => "$basedir/src/jubatus",
);

%moduleheaders = ( # restrict the module headers to those found in relative path
);

%classnames = (
    "qjubatusclassifier.h" => "QJubatusClassifier"
);
%dependencies = (
    "qtbase" => "refs/heads/dev",
);
