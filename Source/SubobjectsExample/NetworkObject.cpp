// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkObject.h"

#include "Net/UnrealNetwork.h"

int32 UNetworkObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	check(GetOuter() != nullptr);
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UNetworkObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));

	if (AActor* Owner = GetTypedOuter<AActor>())
	{
		if (UNetDriver* NetDriver = Owner->GetNetDriver())
		{
			NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
			return true;
		}
	}
	return false;
}

void UNetworkObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UNetworkObject, DefinitionHandle, COND_InitialOnly);

	if (const UBlueprintGeneratedClass* BlueprintClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BlueprintClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

void UNetworkObject::Initialize(const FDataTableRowHandle& Handle)
{
	DefinitionHandle = Handle;
}

void UItemObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// push model here
	DOREPLIFETIME(UItemObject, NumberState);
}

const FItemObjectDefinition& UItemObject::GetDefinition() const
{
	if (Cache == nullptr)
	{
		if (const FItemObjectDefinition* FindIt = DefinitionHandle.GetRow<FItemObjectDefinition>(
			TEXT("UItemObject::GetDefinition")))
		{
			Cache = FindIt;
		}
		else
		{
			//check(TEXT("[%s] Definition Handle must be valid!!"), *DefinitionHandle.RowName.ToString())
			static FItemObjectDefinition s_default;
			Cache = &s_default;
		}
	}
	return *Cache;
}

void UItemObject::SetNumberState(int32 NewValue)
{
	NumberState = NewValue;
	OnPropertyChanged.Broadcast(TEXT("NumberState"));
}

int32 UItemObject::GetNumberState() const
{
	return NumberState;
}

void UItemObject::OnRep_NumberState() const
{
	OnPropertyChanged.Broadcast(TEXT("NumberState"));
}

