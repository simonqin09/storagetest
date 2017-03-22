#include <windows.h>
#include <storemgr.h>


void Partion_Chk_Handle_Else_Return(HANDLE store)
{
    if (store != INVALID_HANDLE_VALUE)
	{
		printf("successfully open store!\n");
	}
	else
	{	
		printf("failed open store!\n");
	}

}

void Partion_Chk_TRUE_Else_Return(BOOL value)
{

	if (value != FALSE)
	{
		printf("successful!\n");
	}
	else
	{	
		printf("failed!\n");
	}
}

int wmain(void)
{
	printf("start to open Disk and dismount it!\n");
	STOREINFO si;
    si.cbSize = sizeof(si);
    HANDLE hStore=::OpenStore(L"DSK1:");
    Partion_Chk_Handle_Else_Return(hStore);
	Partion_Chk_TRUE_Else_Return(DismountStore(hStore));
    Partion_Chk_TRUE_Else_Return(GetStoreInfo(hStore,&si));

    printf("\nstart to find original partition!\n");
	PARTINFO pi;
    pi.cbSize = sizeof(pi);
    HANDLE hQueryPart= FindFirstPartition(hStore, &pi);
    Partion_Chk_Handle_Else_Return(hQueryPart);
    FindClosePartition(hQueryPart); 
    /*if (pi.snNumSectors > (si.snNumSectors/2))
    {
        return true;
    }*/
 
    printf("\nstart to open original partition!\n");
	HANDLE hPart = OpenPartition(hStore,pi.szPartitionName);
    Partion_Chk_Handle_Else_Return(hPart);
    Partion_Chk_TRUE_Else_Return(GetPartitionInfo(hPart,&pi));
 
    printf("\nstart to delete original partition!\n");
	Partion_Chk_TRUE_Else_Return(DeletePartition(hPart,pi.szPartitionName));
	Partion_Chk_TRUE_Else_Return(GetStoreInfo(hStore,&si));
    
	printf("\nstart to create reserved partition!\n");
	Partion_Chk_TRUE_Else_Return(CreatePartition(hStore,_T("boot"),303156));
	Partion_Chk_TRUE_Else_Return(GetStoreInfo(hStore,&si));
    
	printf("\nstart to create extended FS partition!\n");
	//Partion_Chk_TRUE_Else_Return(CreatePartition(hStore,_T("FlashDisk"),7430031));
    Partion_Chk_TRUE_Else_Return(CreatePartition(hStore,pi.szPartitionName,7430031));
	Partion_Chk_TRUE_Else_Return(GetStoreInfo(hStore,&si));

    printf("\nstart to find the reserved partition!\n");
	hQueryPart= FindFirstPartition(hStore, &pi);
    Partion_Chk_Handle_Else_Return(hQueryPart);
    FindClosePartition(hQueryPart); 
    
	printf("\nstart to open the reserved partition!\n");
	hPart = OpenPartition(hStore,pi.szPartitionName);
	Partion_Chk_Handle_Else_Return(hPart);
    Partion_Chk_TRUE_Else_Return(GetPartitionInfo(hPart,&pi));

    printf("\nstart to delete the reserved partition!\n");
	Partion_Chk_TRUE_Else_Return(DismountPartition(hPart));
	Partion_Chk_TRUE_Else_Return(DeletePartition(hPart,pi.szPartitionName));
	Partion_Chk_TRUE_Else_Return(GetStoreInfo(hStore,&si));
 
    printf("\nstart to find the new extended FS partition!\n");
	hQueryPart= FindFirstPartition(hStore, &pi);
    Partion_Chk_Handle_Else_Return(hQueryPart);
    FindClosePartition(hQueryPart);

    printf("\nstart to open the new extended FS partition and format!\n");
	hPart = OpenPartition(hStore,pi.szPartitionName);
	Partion_Chk_Handle_Else_Return(hPart);
    Partion_Chk_TRUE_Else_Return(GetPartitionInfo(hPart,&pi));
	Partion_Chk_TRUE_Else_Return(FormatPartition(hPart));
    
	getchar();
    return TRUE;

}