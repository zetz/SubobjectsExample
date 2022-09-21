// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "NetworkObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyPropertyChangeDelegate, FName, PropName);
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SUBOBJECTSEXAMPLE_API UNetworkObject : public UObject
{
	GENERATED_BODY()

protected:
	// subobject replication
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) { return false; }
	// subobject replication

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "NetworkObject")
	void Initialize(const FDataTableRowHandle& Handle);

	UPROPERTY(BlueprintAssignable, Category = "NetworkObject")
	FNotifyPropertyChangeDelegate OnPropertyChanged;

protected:
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "NetworkObject")
	FDataTableRowHandle DefinitionHandle;
};


USTRUCT()
struct FItemObjectDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FText DisplayName;

};


UCLASS(Blueprintable)
class UItemObject : public UNetworkObject
{
	GENERATED_BODY()

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	const FItemObjectDefinition& GetDefinition() const;
private:
	mutable const FItemObjectDefinition* Cache;


public:
	void SetNumberState(int32 NewValue);
	int32 GetNumberState() const;
private:
	UPROPERTY(ReplicatedUsing = OnRep_NumberState)
	int32 NumberState;

	UFUNCTION()
	void OnRep_NumberState() const;
};

