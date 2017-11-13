#include "risipioscontactaccessmanager.h"
#include "risipcontactmanager.h"
#include "risipmodels.h"

//#inclusw <QImage>
#include <QDebug>

//ios imports
#import <UiKit/UIImage.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSObject.h>
#import <Contacts/Contacts.h>

//defining class ContactScan that will do the fetching of contacts
//from an ios device
@interface ContactsScan:NSObject
{
    RisipiOSContactAcessManager *m_handler;
}
- (ContactsScan*) init: ( RisipiOSContactAcessManager *)handler;
- (void) contactScan;
- (void) getAllContact;
- (void) parseContactWithContact :(CNContact* )contact;
@end

@implementation ContactsScan

- (ContactsScan*) init: ( RisipiOSContactAcessManager*)handler
{
    m_handler = handler;
    return self;
}
//NOTE: iOS 9 or later
//asking for permission to access the device contacts.
- (void) contactScan
{
    if ([CNContactStore class]) {
        //ios9 or later
        CNEntityType entityType = CNEntityTypeContacts;
        if( [CNContactStore authorizationStatusForEntityType:entityType] == CNAuthorizationStatusNotDetermined)
        {
            CNContactStore * contactStore = [[CNContactStore alloc] init];
            [contactStore requestAccessForEntityType:entityType completionHandler:^(BOOL granted, NSError * _Nullable error) {
                if (granted == YES){
                    [self getAllContact];
                }
            }];
        }
        else if( [CNContactStore authorizationStatusForEntityType:entityType]== CNAuthorizationStatusAuthorized)
        {
            [self getAllContact];
        }
    }
}

//fetching all the contacts from the device
//this function is called if the permission is granted.
-(void)getAllContact
{
    if([CNContactStore class])
    {
        //NOTE iOS 9 or later
        NSError* contactError;
        CNContactStore* addressBook = [[CNContactStore alloc]init];
        [addressBook containersMatchingPredicate:[CNContainer predicateForContainersWithIdentifiers: @[addressBook.defaultContainerIdentifier]] error:&contactError];
        NSArray * keysToFetch =@[CNContactEmailAddressesKey, CNContactPhoneNumbersKey, CNContactFamilyNameKey, CNContactGivenNameKey, CNContactPostalAddressesKey, CNContactImageDataAvailableKey, CNContactImageDataKey];
                CNContactFetchRequest * request = [[CNContactFetchRequest alloc]initWithKeysToFetch:keysToFetch];
                BOOL success = [addressBook enumerateContactsWithFetchRequest:request error:&contactError usingBlock:^(CNContact * __nonnull contact, BOOL * __nonnull stop){
            [self parseContactWithContact:contact];
        }];
        if (contactError) {
//            NSLog(@"error fetching contacts %@", contactError);
        }
    }
}

//parsing a retrieved contact
//function is called for each contact that is retrieved in the getAllContact function
- (void)parseContactWithContact :(CNContact* )contact
{
    RisipPhoneContact *risipPhoneContact = new RisipPhoneContact;
    risipPhoneContact->setContactId(QString::fromNSString(contact.identifier).toInt());

    if(contact.imageDataAvailable) {
        NSData *imgData = contact.imageData;
        QByteArray imagedata = QByteArray::fromNSData(imgData);
        risipPhoneContact->setContactImageData(imagedata);
    }

    QString name = QString("%1 %2")
            .arg(QString::fromNSString(contact.givenName))
            .arg(QString::fromNSString(contact.familyName));
    risipPhoneContact->setFullName(name);

    NSArray<CNLabeledValue<CNPhoneNumber *> *> *phNos= contact.phoneNumbers;
    for (id object in phNos) {
        NSString* label = [object label];
        label = [CNLabeledValue localizedStringForLabel:label];
        NSString* number = [[object value] stringValue];

        if (number) {
            if(label)
                risipPhoneContact->addPhoneNumber(QString::fromNSString(number), QString::fromNSString(label));
            else
                risipPhoneContact->addPhoneNumber(QString::fromNSString(number));
        }
    }

    //NOTE Emit contact data to recieve it on the Qt thread.
    m_handler->phoneContactDiscovered(risipPhoneContact);
}

@end

/**
 * @brief The RisipiOSCOntactPrivate class
 *
 * Private class to handle the iOS function calls.
 */
class RisipiOSContactPrivate
{
private:
    RisipiOSContactPrivate(RisipiOSContactAcessManager *parentInstance);
    ~RisipiOSContactPrivate();

    void fetchContacts();

private:
    RisipiOSContactAcessManager* parent;
    ContactsScan *scanner;
    friend class RisipiOSContactAcessManager;
};

RisipiOSContactPrivate::RisipiOSContactPrivate(RisipiOSContactAcessManager *parentInstance)
    :parent(parentInstance)
{
    scanner = [[ContactsScan alloc] init:parent];
}

RisipiOSContactPrivate::~RisipiOSContactPrivate()
{
    [scanner release];
}

void RisipiOSContactPrivate::fetchContacts()
{
    [scanner contactScan];
}

RisipiOSContactAcessManager::RisipiOSContactAcessManager(QObject *parent)
    :QObject(parent)
{

#ifdef Q_OS_IOS
    m_d = new RisipiOSContactPrivate(this);
#endif

}

void RisipiOSContactAcessManager::fetchContactsFromDevice()
{

#ifdef Q_OS_IOS
    m_d->fetchContacts();
#endif

}
