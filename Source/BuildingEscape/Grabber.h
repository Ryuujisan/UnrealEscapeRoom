// CopyRight Ryuu Sakurai 2018

#pragma once
#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Grab();
	void Relase();

private:
	float reach = 100.f;
	//UPhysicsHandleComponent * physickHandle = nullptr;
	UPhysicsHandleComponent* physickHandle = nullptr;
	UInputComponent* inputComponent;
};

