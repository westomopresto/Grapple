from subprocess import Popen


def buildGame():

    p = Popen("BuildGame.bat", cwd=r"C:\Users\Westo\Documents\GitHub\Grapple\BUILD_TOOLS")
    stdout, stderr = p.communicate()
    
def uploadToSteam():
    p = Popen("steam_upload_only.bat", cwd=r"C:\Users\Westo\Documents\Fadeout_Builds\2022")
    stdout, stderr = p.communicate()

buildGame()
uploadToSteam()