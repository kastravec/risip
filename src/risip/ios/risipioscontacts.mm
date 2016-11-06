#include "risipioscontacts.h"
#include "risipphonecontact.h"
#include "risipcontactmanager.h"
#include "risipmodels.h"

#include <QDebug>

//ios imports
#import <Foundation/NSArray.h>
#import <Foundation/NSObject.h>
#import <Contacts/Contacts.h>

//defining class ContactScan that will do the fetching of contacts
//from an ios device
@interface ContactsScan:NSObject
{
                            RisipiOSContacts *m_handler;
}
- (ContactsScan*) init: ( RisipiOSContacts *)handler;
- (void) contactScan;
- (void) getAllContact;
- (void) parseContactWithContact :(CNContact* )contact;
- (NSMutableArray *) parseAddressWithContac: (CNContact *)contact;
@end

@implementation ContactsScan

- (ContactsScan*) init: ( RisipiOSContacts*)handler
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
        NSArray * keysToFetch =@[CNContactEmailAddressesKey, CNContactPhoneNumbersKey, CNContactFamilyNameKey, CNContactGivenNameKey, CNContactPostalAddressesKey];
        CNContactFetchRequest * request = [[CNContactFetchRequest alloc]initWithKeysToFetch:keysToFetch];
        BOOL success = [addressBook enumerateContactsWithFetchRequest:request error:&contactError usingBlock:^(CNContact * __nonnull contact, BOOL * __nonnull stop){
            [self parseContactWithContact:contact];
        }];
        if (contactError) {
            NSLog(@"error fetching contacts %@", contactError);
        }
    }
}

//parsing a retrieved contact
//function is called for each contact that is retrieved in the getAllContact function
- (void)parseContactWithContact :(CNContact* )contact
{
    NSString *identifier = contact.identifier;
    NSString *firstName =  contact.givenName;
    NSString *lastName =  contact.familyName;

    NSArray<CNLabeledValue<CNPhoneNumber *> *> *phNos= contact.phoneNumbers;
    QStringList phoneNumbers;
    for (id object in phNos) {
        NSString* label = [object label];
        label = [CNLabeledValue localizedStringForLabel:label];
        NSString* number = [[object value] stringValue];
        if (label && number) {
            phoneNumbers<<QString::fromNSString(label)<<QString::fromNSString(number);
        }
    }

    //NSString * phone = [[contact.phoneNumbers valueForKey:@"value"] valueForKey:@"digits"];
    //NSString * email = [contact.emailAddresses valueForKey:@"value"];
    //NSArray * addrArr = [self parseAddressWithContac:contact];

    QString name = QString("%1 %2")
            .arg(QString::fromNSString(firstName))
            .arg(QString::fromNSString(lastName));

    RisipPhoneContact *risipPhoneContact = new RisipPhoneContact;
    risipPhoneContact->setFullName(name);
    risipPhoneContact->setContactId(QString::fromNSString(identifier).toInt());
    risipPhoneContact->setPhoneNumbers(phoneNumbers);

    //NOTE Emit contact data to recieve it on the Qt thread.
    m_handler->phoneContactDiscovered(risipPhoneContact);
}

- (NSMutableArray *)parseAddressWithContac: (CNContact *)contact
{
    NSMutableArray * addrArr = [[NSMutableArray alloc]init];
    CNPostalAddressFormatter * formatter = [[CNPostalAddressFormatter alloc]init];
    NSArray * addresses = (NSArray*)[contact.postalAddresses valueForKey:@"value"];
    if (addresses.count > 0) {
        for (CNPostalAddress* address in addresses) {
            [addrArr addObject:[formatter stringFromPostalAddress:address]];
        }
    }
    return addrArr;
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
     RisipiOSContactPrivate(RisipiOSContacts *parentInstance);
    ~RisipiOSContactPrivate();

     void fetchContacts();

private:
     RisipiOSContacts* parent;
     ContactsScan *scanner;
     friend class RisipiOSContacts;
};

RisipiOSContactPrivate::RisipiOSContactPrivate(RisipiOSContacts *parentInstance)
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

RisipiOSContacts::RisipiOSContacts(QObject *parent)
    :QObject(parent)
{
    m_d = new RisipiOSContactPrivate(this);
}

void RisipiOSContacts::fetchContactsFromDevice()
{
    m_d->fetchContacts();
}
