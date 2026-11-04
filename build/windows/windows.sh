
LOGNAME=$(whoami)

echo
echo Package file
echo ============
ls -la windows.xml

echo
echo Package with install builder
echo ============================
/bin/rm -f /c/Users/${LOGNAME}/Documents/InstallBuilder/output/*exe
"/c/Program Files/InstallBuilder Professional 26.8.0/bin/builder-cli.exe" build windows.xml
if [[ $? -ne 0 ]]; then
    exit 1
fi

echo
echo /c/Users/${LOGNAME}/Documents/InstallBuilder/output
echo ========================================================
ls -la /c/Users/${LOGNAME}/Documents/InstallBuilder/output/
cp /c/Users/${LOGNAME}/Documents/InstallBuilder/output/* ../..
cp /c/Users/${LOGNAME}/Documents/InstallBuilder/output/* /c/Users/${LOGNAME}/Desktop

echo
echo /c/Users/${LOGNAME}/Desktop
echo ========================================================
ls -la /c/Users/${LOGNAME}/Desktop/*acrecant*exe

echo
echo Packaging complete
echo
exit 0
