// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkObject.h"
#include "Components/ActorComponent.h"
#include "NetworkObjectContainer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyInventoryChangeDelegate, const TArray<UNetworkObject*>&, Added,
		const TArray<UNetworkObject*>&, Removed);

UCLASS(Abstract, Blueprintable)
class SUBOBJECTSEXAMPLE_API UNetworkObjectContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkObjectContainer();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UPROPERTY(ReplicatedUsing = OnRep_Items, VisibleInstanceOnly, BlueprintReadOnly, Category = "NetworkObject")
	TArray<UNetworkObject*> Items;

	UPROPERTY(BlueprintAssignable, Category = "NetworkObject")
	FNotifyInventoryChangeDelegate OnInventoryChanged;

	UFUNCTION()
	void OnRep_Items(const TArray<UNetworkObject*>& Previous) const;


	UNetworkObject* CreateNetworkObject(TSubclassOf<UNetworkObject> ClassType, FDataTableRowHandle Handle);

	bool DestroyNetworkObject(UNetworkObject* Instance);
};


class UItemInventoryComponent : UNetworkObjectContainer
{
	
};