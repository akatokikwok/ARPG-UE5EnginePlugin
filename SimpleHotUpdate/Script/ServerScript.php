<?php

function CreateDirAndCheck($dir)
{
    if(!is_dir($dir))
    {
        mkdir($dir,0700,true);
    }
    else
    {
        echo $dir."已经存在~~~~\n";
    }
}

$Protocol = $_GET["Protocol"];

//HSP_INIT,//初始化服务器的数据
if($Protocol == 0)
{    
    $Bucket = $_GET["Bucket"];
    $PatchVersionLogName = $_GET["PatchVersionLogName"];
    $ServerVersionName = $_GET["ServerVersionName"];
    $VersionLock = $_GET["VersionLock"];
    $Platform = $_GET["Platform"];

    $PatchVersionLogName = "../".$Bucket."/".$Platform."/".$PatchVersionLogName;
    $ServerVersionName = "../".$Bucket."/".$Platform."/".$ServerVersionName;
    $VersionLock = "../".$Bucket."/".$Platform."/".$VersionLock;
    $Patch = "../".$Bucket."/".$Platform."//Patch//";

    CreateDirAndCheck($PatchVersionLogName);
    CreateDirAndCheck($ServerVersionName);
    CreateDirAndCheck($VersionLock);
    CreateDirAndCheck($Patch);
}
//HSP_CHECK_LOCK,//检测锁
else if($Protocol == 1)
{
    $VersionLockPath = "../".$_GET["VersionLockPath"];
    if(!file_exists($VersionLockPath))
    {
        file_put_contents($VersionLockPath,"false");
        echo "false";
    }
    else
    {
        echo file_get_contents($VersionLockPath);
    }
}
//	HSP_LOCK,//上锁
else if($Protocol == 2)
{
    $VersionLockPath = "../".$_GET["VersionLockPath"];
    if(!file_exists($VersionLockPath))
    {
        file_put_contents($VersionLockPath,"true");  
    }

    file_put_contents($VersionLockPath,"true");
    echo "false";
}
//	HSP_UNLOCK,//解锁
else if($Protocol == 3)
{
   $VersionLockPath = "../".$_GET["VersionLockPath"];
    if(!file_exists($VersionLockPath))
    {
        file_put_contents($VersionLockPath,"false");  
    }
    
    file_put_contents($VersionLockPath,"false");
    echo "false";
}


