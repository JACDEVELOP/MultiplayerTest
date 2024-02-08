// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_CharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiplayerTest/UI/MP_UserName.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMP_CharacterBase

AMP_CharacterBase::AMP_CharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetOwnerNoSee(true);
	WidgetComponent->SetIsReplicated(true);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMP_CharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMP_CharacterBase, ActorToInteract);
}

void AMP_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	Server_SetUserName();
}

void AMP_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Server_SetUserName();
}

void AMP_CharacterBase::Server_Interact_Implementation()
{
	if(!IsValid(ActorToInteract))
	{
		return;
	}
	if(ActorToInteract->GetClass()->ImplementsInterface(UMP_InteractInterface::StaticClass()))
	{
		IMP_InteractInterface::Execute_Interact(ActorToInteract);
	}
}

void AMP_CharacterBase::Server_SetUserName_Implementation()
{
	Multicast_SetUserName();
}

void AMP_CharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMP_CharacterBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Check if the character is not controlled locally (e.g., other players in a multiplayer game)
    if (!IsLocallyControlled())
    {
	    if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
        {
            // Get the camera and widget locations
            const FVector CameraManagerLocation = CameraManager->GetCameraLocation();
            const FVector WidgetLocation = WidgetComponent->GetComponentLocation();

            // Calculate the rotation from the widget to the camera
            const FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraManagerLocation);

            // Set the widget's rotation to face the camera
            WidgetComponent->SetWorldRotation(PlayerRot);
        }
    }
    else
    {
        // Handling raycast for local player
        FVector CameraLocation = FollowCamera->GetComponentLocation();
        FVector End = CameraLocation + (FollowCamera->GetForwardVector() * 900);

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);

        // Perform a raycast from the camera forward
        if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility, CollisionParams))
        {
            AActor* HitActor = HitResult.GetActor();

            // Check if the hit actor is valid and implements the interaction interface
            if (IsValid(HitActor) && UKismetSystemLibrary::DoesImplementInterface(HitActor, UMP_InteractInterface::StaticClass()))
            {
                Server_SetInteractableActor(HitActor);

                // Enable interaction widget if the actor to interact with implements the interface
                if (IsValid(ActorToInteract) && ActorToInteract->GetClass()->ImplementsInterface(UMP_InteractInterface::StaticClass()))
                {
                    IMP_InteractInterface::Execute_EnableInteractWidget(ActorToInteract, true);
                }
            }
            else
            {
                // Disable interaction widget and set interactable actor to nullptr if not valid
                if (IsValid(ActorToInteract) && ActorToInteract->GetClass()->ImplementsInterface(UMP_InteractInterface::StaticClass()))
                {
                    IMP_InteractInterface::Execute_EnableInteractWidget(ActorToInteract, false);
                }
                Server_SetInteractableActor(nullptr);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMP_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMP_CharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMP_CharacterBase::Look);

		// Looking
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMP_CharacterBase::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMP_CharacterBase::Multicast_SetUserName_Implementation()
{
	if(!IsValid(WidgetComponent))
	{
		return;
	}
	const UMP_UserName* UserName = Cast<UMP_UserName>(WidgetComponent->GetWidget());
	if(!IsValid(UserName))
	{
		return;
	}
	if(!IsValid(GetPlayerState()))
	{
		return;
	}
	UserName->SetPlayerName(FText::FromString(GetPlayerState()->GetPlayerName()));
}

void AMP_CharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMP_CharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMP_CharacterBase::Interact(const FInputActionValue& Value)
{
	if(!IsValid(ActorToInteract))
	{
		return;
	}
	Server_Interact();
}

void AMP_CharacterBase::Server_SetInteractableActor_Implementation(AActor* NewActor)
{
	ActorToInteract = NewActor;
}
