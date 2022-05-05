# web-admin-boilerplate

## First run
```
$ npm ci
```

## Start in developer mode
```
$ npm start
```

## Compile build
```
$ npm run build
```

Run local server with production build for local testing:
```
$ npm run start:prod
```

# Usage

## Add new pages
1) Add `.html` file to `src/` dir
2) Add `.html` file with JS to `src/js` dir
3) Add path to file in `header.html`

## Dynamic styles
Classes that added to elements in runtime should be added to `dynamiicStyles.html` as parcel remove css styles from final build that are not found in html while compiling process.
